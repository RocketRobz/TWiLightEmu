#ifndef DS_SYSTEM_H
#define DS_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

extern u32 dsArm9Regs[16];
extern u32 dsArm7Regs[16];

extern u8 dsMainRam[0x400000];
extern u8 dsVram[0xA4000];
extern u8 dsSharedWram[0x8000];
extern u8 dsArm7Wram[0x10000];

#ifdef __cplusplus
}
#endif

#endif // DS_SYSTEM_H
