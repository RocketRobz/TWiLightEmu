#include <stdio.h>
#include <3ds.h>
#include <string.h>
#include "ds_system.h"

int regL = 0, regR = 0;

u32 fixedRegR;

u16 offsetChange;
u8 opcodeLastByte;

u32 opcodeAlt;

void opMov() { // mov rL, #?
	u8 increaseSwitch = 0;
	while (opcodeAlt >= 0x00000100) {
		opcodeAlt -= 0x00000100;
		increaseSwitch++;
	}

	u32 add = 0;
	switch (increaseSwitch) {
		case 0:
		default:
			break;
		case 1:
			add = 0x40000000;
			break;
		case 2:
			add = 0x10000000;
			break;
		case 3:
			add = 0x4000000;
			break;
		case 4:
			add = 0x1000000;
			break;
		case 5:
			add = 0x400000;
			break;
		case 6:
			add = 0x100000;
			break;
		case 7:
			add = 0x40000;
			break;
		case 8:
			add = 0x10000;
			break;
		case 9:
			add = 0x4000;
			break;
		case 0xA:
			add = 0x1000;
			break;
		case 0xB:
			add = 0x400;
			break;
		case 0xC:
			add = 0x100;
			break;
		case 0xD:
			add = 0x40;
			break;
		case 0xE:
			add = 0x10;
			break;
		case 0xF:
			add = 0x4;
			break;
	}
	dsArm9Regs[regL] = opcodeLastByte*add;
}

void setArmPc(const bool arm7, const u32 offset) {
	if (arm7) {
		dsArm7Regs[15] = offset;
		return;
	}
	dsArm9Regs[15] = offset;
}

bool armInterpreter(void) {
	u32 opcode = dsSystem_offsetAdjust(dsArm9Regs[15]);
	opcode = *(u32*)opcode;

	u32 opcodePushPop = opcode;

	{
		while (opcode >= 0x10000000) {
			opcode -= 0x10000000;
			opcodePushPop -= 0x10000000;
		}

		u32 opcodeTemp = opcode;
		while (opcodeTemp >= 0x01000000) {
			opcodeTemp -= 0x01000000;
		}

		while (opcodeTemp >= 0x00100000) {
			opcodeTemp -= 0x00100000;
		}

		while (opcodeTemp >= 0x00010000) {
			opcodeTemp -= 0x00010000;
			opcode -= 0x00010000;
			opcodePushPop -= 0x00010000;
			regR++;
		}

		while (opcodeTemp >= 0x00001000) {
			opcodeTemp -= 0x00001000;
			opcode -= 0x00001000;
			regL++;
		}
	}

	fixedRegR = dsSystem_offsetAdjust(dsArm9Regs[regR]);

	offsetChange = (u16)opcode;
	opcodeLastByte = (opcode & 0xFF);

	opcodeAlt = opcode;
	opcode -= offsetChange;

	if (opcode == 0x03A00000) { // mov rL, #?
		opMov();
	} else
	if (opcodePushPop == 0x09900003) { // ldmib [rR], r0, r1
		dsArm9Regs[0] = *(u32*)(fixedRegR+4);
		dsArm9Regs[1] = *(u32*)(fixedRegR+8);
	} else
	if (opcodePushPop == 0x09800003) { // stmib [rR], r0, r1
		*(u32*)(fixedRegR+4) = dsArm9Regs[0];
		*(u32*)(fixedRegR+8) = dsArm9Regs[1];
	} else
	if (opcode == 0x05900000) { // ldr rL, [rR]
		dsArm9Regs[regL] = *(u32*)(fixedRegR+offsetChange);
	} else
	if (opcode == 0x05800000) { // str rL, [rR]
		*(u32*)(fixedRegR+offsetChange) = dsArm9Regs[regL];
	} else
	if (opcode == 0x05100000) { // ldr rL, [rR, #-?]
		dsArm9Regs[regL] = *(u32*)(fixedRegR-offsetChange);
	} else
	if (opcode == 0x05000000) { // str rL, [rR, #-?]
		*(u32*)(fixedRegR-offsetChange) = dsArm9Regs[regL];
	} else
	if (opcode == 0x04900000) { // ldr rL, [rR],#0-#0xFFF
		dsArm9Regs[regL] = *(u32*)fixedRegR;
		dsArm9Regs[regR] += offsetChange;
	} else
	if (opcode == 0x04800000) { // str rL, [rR],#0-#0xFFF
		*(u32*)fixedRegR = dsArm9Regs[regL];
		dsArm9Regs[regR] += offsetChange;
		return true;
	} else
	if (opcodeAlt >= 0x07900000 && opcodeAlt <= 0x0790000C) { // ldr rL, [rR, r0-r12]
		dsArm9Regs[regL] = *(u32*)(fixedRegR+dsArm9Regs[opcodeLastByte]);
	} else
	if (opcodeAlt >= 0x07800000 && opcodeAlt <= 0x0780000C) { // str rL, [rR, r0-r12]
		*(u32*)(fixedRegR+dsArm9Regs[opcodeLastByte]) = dsArm9Regs[regL];
	} else
	if ((opcode == 0x01D00000 || opcode == 0x01C00000) && opcodeLastByte >= 0xB0 && opcodeLastByte <= 0xBF) { // ldrh/strh rL, [rR]
		u8 offsetChange = opcodeLastByte - 0xB0;

		opcodeAlt -= opcode;

		while (opcodeAlt >= 0x00000100) {
			opcodeAlt -= 0x00000100;
			offsetChange += 0x10;
		}

		if (opcode == 0x01D00000) {
			dsArm9Regs[regL] = *(u16*)(fixedRegR+offsetChange);
		} else {
			*(u16*)(fixedRegR+offsetChange) = (u16)dsArm9Regs[regL];
		}
	} else
	if ((opcode == 0x00D00000 || opcode == 0x00C00000) && opcodeLastByte >= 0xB0 && opcodeLastByte <= 0xBF) { // ldrh/strh rL, [rR],#0-#0xFF
		if (opcode == 0x00D00000) {
			dsArm9Regs[regL] = *(u16*)fixedRegR;
		} else {
			*(u16*)fixedRegR = (u16)dsArm9Regs[regL];
		}
		u8 regAddCount = opcodeLastByte - 0xB0;

		opcodeAlt -= opcode;

		while (opcodeAlt >= 0x00000100) {
			opcodeAlt -= 0x00000100;
			regAddCount += 0x10;
		}

		dsArm9Regs[regR] += regAddCount;
	} else
	if (opcode == 0x05D00000) { // ldrb rL, [rR]
		dsArm9Regs[regL] = *(u8*)(fixedRegR+offsetChange);
	} else
	if (opcode == 0x04D00000) { // ldrb rL, [rR],#0-#0xFFF
		dsArm9Regs[regL] = *(u8*)fixedRegR;
		dsArm9Regs[regR] += offsetChange;
	} else
	if ((opcode == 0x07D00000) && ((opcodeLastByte >= 0x00 && opcodeLastByte <= 0x0C) || (opcodeLastByte >= 0x80 && opcodeLastByte <= 0x8C))) { // ldrb rL, [rR, r0-r12, lsl #?]
		u8 lsl = (opcodeLastByte >= 0x80 && opcodeLastByte <= 0x8C) ? 1 : 0;

		opcodeAlt -= opcode;

		while (opcodeAlt >= 0x00000100) {
			opcodeAlt -= 0x00000100;
			lsl += 2;
		}

		u32 add = dsArm9Regs[opcodeLastByte - 0x80];
		while (lsl > 1) {
			add += add;
			lsl--;
		}

		dsArm9Regs[regL] = *(u8*)(fixedRegR+add);
	} else {
		iprintf("Unimplemented/Unknown instruction!\n\n");

		char offsetText[64];
		sprintf(offsetText, "Offset: %08lx\nOpcode: %08lx\n", dsArm9Regs[15], opcode);
		iprintf(offsetText);
		return false;
	}

	dsArm9Regs[15] += 4; // Update pc
	return true;
}
