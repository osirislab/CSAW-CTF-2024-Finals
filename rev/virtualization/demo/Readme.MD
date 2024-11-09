# Project Name

This project is designed to securely hide a flag within a compiled application using bitwise operations and custom bytecode. Below is a breakdown of each development step, detailing the purpose of each file and the process to create and verify the final executable.

## Development Progress

### Step 1: Encrypting the Flag (`encrypt.c` and `genFlag.py`)

To begin, I created two files:
- **`encrypt.c`**: Responsible for setting up a secure, static hiding mechanism for the flag.
- **`genFlag.py`**: Generates and outputs an encrypted flag using a combination of bit shifts and XOR operations. This encryption routine ensures the flag remains hidden without risking leaks during runtime.

> **Note**: The output from this process can be found in `notes.txt`, where the obfuscated numbers are documented for reference.

### Step 2: Adding a Gate and Message (`app.asm`)

Next, I wrote **`app.asm`**, which incorporates:
1. The previously hidden flag.
2. A "gate" mechanism that verifies access.

This assembly file further protects the message by XORing it with a unique string. For this project, I chose the message `"I_LOVE_KOTLIN"` and applied a random string of numbers as the XOR key to ensure obfuscation.

### Step 3: Compilation and Testing

After building the encryption and gate, I compiled the code to verify:
1. The flag decrypts correctly upon execution.
2. The message displays as expected without revealing sensitive data.

The code is slightly tweaked to create `pseudo.asm` which makes making compiler a lot nicer

### Step 4: Custom Bytecode Generation (`compiler.py`)

Finally, I created **`compiler.py`**, which converts the compiled assembly code into a custom bytecode format. This bytecode is saved as `program.json` and serves as the foundation for a virtualized version of the program.

The generated `program.json` is used downstream in the project's virtual machine, enabling further steps in the program's execution.

---

## Files Overview

| File        | Description                                                                                     |
|-------------|-------------------------------------------------------------------------------------------------|
| `encrypt.c` | Contains the encryption logic using bit shifts and XOR to statically hide the flag.             |
| `genFlag.py`| Python script to generate encrypted flag output and save it for later use.                      |
| `notes.txt` | Records the output numbers from `genFlag.py`, essential for tracking the encrypted values.      |
| `app.asm`   | Assembly code that incorporates the encrypted flag and includes a gate-check mechanism.         |
| `compiler.py`| Compiles the assembly code to a bytecode format (`program.json`) for the virtualized program.   |
| `program.json`| JSON file with custom bytecode, created by `compiler.py` and used in the virtual machine.     |

## Usage

1. Run `genFlag.py` to generate the encrypted flag.
2. Use `encrypt.c` to apply the bit shift and XOR techniques on the flag.
3. Compile `app.asm` to verify message and gate functionality.
4. Run `compiler.py` to produce the bytecode in `program.json`.

---

This README outlines each stage of the development process and explains the purpose of each file in this secure flag-hiding project. Follow these steps to reproduce the project setup and testing procedures.
