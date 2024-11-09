# Boss Battle

This challenge uses the Ethereum ctf challenge framework developed by samczsun at Paradigm. It was originally written for CSAW CTF 2022.

See [this blog post](https://www.zellic.io/blog/how-to-create-an-ethereum-ctf-challenge) for more information. Pull requests welcome!

## Installing

### Prerequisites

* Docker
* Python 3

### Configuration
You'll also need to manually install the following:
* `pip install yaml ecdsa pysha3 web3`

## Usage

### Build everything

```bash
docker build --platform linux/amd64 -t challenge:latest .
```

### Run a challenge

Running a challenge will open a port which users will `nc` to. For Ethereum/Starknet related
challenges, an additional port must be supplied so that users can connect to the Ethereum/Starknet
node

```
./run.sh challenge:latest 31337 8545
```

On another terminal:

```
nc localhost 31337
```

When prompted for the ticket, they will need to solve a PoW. This ticket should NOT be shared between teams, it's a secret.

```
$ nc localhost 31337
1 - launch new instance
2 - kill instance
3 - get flag
action? 1
ticket please: ticket

your private blockchain has been deployed
it will automatically terminate in 30 minutes
here's some useful information
```

### How to solve it

See `solution-example.py` and `Exploit.sol` for scaffolding.
