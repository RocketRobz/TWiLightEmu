#ifndef ARM_INTERPRETER_H
#define ARM_INTERPRETER_H

extern void setArmPc(const bool arm7, const u32 offset);
extern bool armInterpreter(void);

#endif // ARM_INTERPRETER_H
