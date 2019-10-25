#include <stack.h>

/* Initializes stack */
StackFrame initStack(void * entryPoint, void * stackBase) {
    StackFrame ret;
    ret.r15 = 0x0;
	ret.r14 = 0x0;
	ret.r13 = 0x0;
	ret.r12 = 0x0;
	ret.r11 = 0x0;
	ret.r10 = 0x0;
	ret.r9 = 0x0;
	ret.r8 = 0x0;
	ret.rsi = 0x0;
	ret.rdi = 0x0;
	ret.rbp = 0x0;
	ret.rdx = 0x0;
	ret.rcx = 0x0;
	ret.rbx = 0x0;
	ret.rax = 0x0;
    ret.ip = (uint64_t) entryPoint;
    ret.cs = 0x8;
    ret.eflags = 0x202;
    ret.sp = (uint64_t) stackBase;
    ret.ss = 0x0;
    ret.returnAddress = (uint64_t)&killCurrent;

    return ret;
}