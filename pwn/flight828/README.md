
# CSAW-CTF-2024 Finals - PWN Challenge: Flight828

## Instructions
### 1. Download the required files
Download the binary (repeat), docker file and flag.txt. 

### 2. Build the Docker Image
Run the following commands to build and run the Docker image:

```bash
docker build -t ctf-challenge .
docker run -d -p 5454:5454 --name ctf-challenge ctf-challenge
```

### 3. Test Remotely - Connect to the Challenge and Play
Use the following command to connect:

```bash
socat - TCP:localhost:5454
```
(Please dont use nc , it's stubborn and doesn't want to close after sending the payload)
### 4. Test Locally
To test locally, run `./repeat`.

Compiling the source code is not necessary or recommended, but if you insist compile it like this:

```bash
gcc -fno-stack-protector -fno-pie -no-pie -O0 -o repeat repeat.c
```
## Solution
```bash
python3 solver_script_remote.py
```
