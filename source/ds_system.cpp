﻿#include <3ds.h>

u32 dsArm9Regs[16];
u32 dsArm7Regs[16];

u8* dsArm9Io;

u8* dsMainRam;
u8* dsVram;
u8* dsSharedWram;
u8* dsArm7Wram;

u32 dsSystem_offsetAdjust(u32 reg) {
	if (reg >= 0x02000000 && reg < 0x03000000) {
		while (reg >= 0x02400000) {
			reg -= 0x400000; // Un-mirror the offset
		}
		reg -= 0x02000000;
		reg += (u32)dsMainRam;
	} else if (reg >= 0x04000000 && reg < 0x04002000) {
		reg -= 0x04000000;
		reg += (u32)dsArm9Io;
	}

	return reg;
}

void dsSystemInit(void) {
	dsArm9Io = (u8*)new u32[0x2000/4];

	dsMainRam = (u8*)new u32[0x400000/4];
	dsVram = (u8*)new u32[0xA4000/4];
	dsSharedWram = (u8*)new u32[0x8000/4];
	dsArm7Wram = (u8*)new u32[0x10000/4];
}

void dsSystemExit(void) {
	delete[] dsArm9Io;

	delete[] dsMainRam;
	delete[] dsVram;
	delete[] dsSharedWram;
	delete[] dsArm7Wram;
}