#ifndef DS_SYSTEM_H
#define DS_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

u32 dsArm9Regs[16];
u32 dsArm7Regs[16];

u32 dsMainRam[0x400000/4];
u16 dsVram[0xA4000/2];
u32 dsSharedWram[0x8000/4];
u32 dsArm7Wram[0x10000/4];

#ifdef __cplusplus
}
#endif

#endif // DS_SYSTEM_H
