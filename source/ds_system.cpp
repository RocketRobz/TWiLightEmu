#include <3ds.h>

u32 dsArm9Regs[16];
u32 dsArm7Regs[16];

u8 dsMainRam[0x400000];
u8 dsVram[0xA4000];
u8 dsSharedWram[0x8000];
u8 dsArm7Wram[0x10000];