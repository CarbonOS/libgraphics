/// <summary>
/// X86 Platform
/// </summary>
#ifdef __i386__
#include "arch-x86.h"
#include <asm/unistd.h>
.intel_syntax noprefix

/* Custom Syscalls */
#define __NR_cmpcall 370

/* ASM Version */
/* int Viewcall(int id, int cmd, void *args); */
ENTRY(Viewcall)
    push ebp
    mov ebp, esp
    push ebx
    push ecx
    push edx
    mov eax, __NR_cmpcall
    mov ebx, [ebp+8] # id
    mov ecx, [ebp+12] # cmd
    mov edx, [ebp+16] # args
    int 0x80
    pop edx
    pop ecx
    pop ebx
    leave
    ret
END(Viewcall)


#endif
