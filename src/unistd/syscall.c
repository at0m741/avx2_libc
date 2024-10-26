#include <config.h>
#include <stdarg.h>

long _syscall(long number, ...) {
    va_list args;
    va_start(args, number);

    unsigned long arg1 = va_arg(args, unsigned long);
    unsigned long arg2 = va_arg(args, unsigned long);
    unsigned long arg3 = va_arg(args, unsigned long);
    unsigned long arg4 = va_arg(args, unsigned long);
    unsigned long arg5 = va_arg(args, unsigned long);
    unsigned long arg6 = va_arg(args, unsigned long);

    va_end(args);

    long ret;
#ifdef __APPLE__
	__asm__ volatile (
		"mov rax, %1 \n"
		"mov rdi, %2 \n"
		"mov rsi, %3 \n"
		"mov rdx, %4 \n"
		"mov r10, %5 \n"
		"mov r8, %6  \n"
		"mov r9, %7  \n"
		"syscall      \n"
		"mov rax, %0  \n"
		: "=r" (ret)
		: "r" (number),
		  "r" (arg1),
		  "r" (arg2),
		  "r" (arg3),
		  "r" (arg4),
		  "r" (arg5),
		  "r" (arg6)
		: "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9", "memory"
	);
	
#else
	__asm__ volatile (
		"mov rax, %1        \n"
		"mov rdi, %2        \n"
		"mov rsi, %3        \n"
		"mov rdx, %4        \n"
		"mov r10, %5        \n"
		"mov r8, %6         \n"
		"mov r9, %7         \n"
		"syscall            \n"
		"mov %0, rax        \n"
		: "=r" (ret)
		: "r" (number),
		  "r" (arg1),
		  "r" (arg2),
		  "r" (arg3),
		  "r" (arg4),
		  "r" (arg5),
		  "r" (arg6)
		: "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9", "memory"
	);
#endif

    if (ret < 0) {
        return ret;
    }

    return ret;
}
