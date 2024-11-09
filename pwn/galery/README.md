# Galery

<details> 
  <summary>Flag</summary>
   csawctf{h4Nd_r0ll1ng_B1tm4p_p4rsErs_1s_fuN!}
</details>

## Description

Rev and Pwn challenge. The binary reads a bmp file, creates a copy and prints the headers.

There is a Stack Based Buffer Overflow vulnerability in the function create_copy() that can be exploited to obtain Remote Code Execution. The BOF can be triggered by crafting a BMP file that contains a larger value for the colors_used attribute.

These are the lines that cause the BOF:

```c
...
unsigned char color_table[256][4];
...
fread(color_table, info_header->colors_used, 4, fp);
```

There is also a glibc leak that the players can use to obtain the base address for glibc.

The attribute colors_used is read from the image's headers and immediately used to determine the number of bytes to be read into the fixed size buffer color_table. There is no bound checking before using colors_used.

After reversing the binary and finding the vulnerability the challenge is straight forward to solve. The player needs to take advantage of the vulnerability by crafting a malicious image that can pass all the program checks and contains a colors_used value big enough to overwrite the return pointer with a ROP chain that spawns a shell.


References for dealing with bmp files:

https://en.wikipedia.org/wiki/BMP_file_format

https://www.digicamsoft.com/bmp/bmp.html

https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-types-of-bitmaps-about


## Tools

To solve the challange the player needs a decompiler like Ghidra or Binja, glibc 2.35 and Python.


## Installation

Dockerfile

## Solution

```bash
./solver_galery.py
```
