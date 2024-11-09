from pwn import *
context.log_level='debug'
context.arch='amd64'
#context.terminal = ['tmux', 'splitw', '-h', '-F' '#{pane_pid}', '-P']
p=process('./app')
ru 		= lambda a: 	p.readuntil(a)
r 		= lambda n:		p.read(n)
sla 	= lambda a,b: 	p.sendlineafter(a,b)
sa 		= lambda a,b: 	p.sendafter(a,b)
sl		= lambda a: 	p.sendline(a)
s 		= lambda a: 	p.send(a)

strr = "I_LOVE_KOTLIN"
xorkey = [181, 182, 39, 21, 160, 184, 179, 236, 30, 233, 42, 47, 1]

send = b"".join((ord(i) ^ j).to_bytes(length=1, byteorder="little") for i,j in zip(strr, xorkey))

p.sendline(send)

p.interactive()
