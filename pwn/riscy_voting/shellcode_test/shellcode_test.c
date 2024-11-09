#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv) 
{
    execve("/bin/sh",0,0);
    return 0;
}

//This example assembly is for calling system("/bin/sh").
//=> 0x10516 <main+30>:   lui a5,0x4e
//   0x1051a <main+34>:   addi    a0,a5,1664
//   0x1051e <main+38>:   jal 0x11188 <puts>
//   0x10522 <main+42>:   lui a5,0x4e
//   0x10526 <main+46>:   addi    a0,a5,1672
//   0x1052a <main+50>:   jal 0x1116a <system>
