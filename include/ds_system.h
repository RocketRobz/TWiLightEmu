#ifndef DS_SYSTEM_H
#define DS_SYSTEM_H

extern u32 dsArm9Regs[16];
extern u32 dsArm7Regs[16];

extern u8* dsMainRam;
extern u8* dsVram;
extern u8* dsSharedWram;
extern u8* dsArm7Wram;

extern u32 dsSystem_offsetAdjust(u32 reg);
extern void dsSystemInit(void);
extern void dsSystemExit(void);

#endif // DS_SYSTEM_H
