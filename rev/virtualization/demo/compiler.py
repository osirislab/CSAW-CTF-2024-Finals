from pwn import *
from sys import argv
import re
import json

if len(argv) != 3:
    print("Usage: python3 compiler.py [mode, 1 = program.h, 2 = else.h] [input_file]")
    exit(0)


ops_dict = {
    
    # two operands
    "mov" :                         1,
    "xor" :                         2,
    "ror" :                         3,
    "cmp" :                         4,
    "TWO_OPERANDS" :                5,

    # one operands
    "jne" :                         6,
    "jmp" :                         7,
    "call" :                        8,
    "loop" :                        9,
    "push" :                        10,
    "pop" :                         11,
    "repe" :                        12,
    "dec" :                         13,
    "inc" :                         14,
    "ONE_OPERANDS" :                15,

    # no operands
    "ret" :                         16,
    "syscall" :                     17,
    "NO_OPERANDS" :                 18
}

mov_mode = {
    "REG_REG": 1,
    "REG_IMM": 2,
    "REG_LAB": 3,
    "REG_PTR": 4,
    "PTR_IMM": 5,
    "PTR_REG": 6,
}

xor_mode = {
    "PTR_REG": 1,
    "REG_REG": 2,
    "REG_PTR": 3
}

ror_mode = {
    "REG_IMM": 1,
    "PTR_IMM": 2
}

cmp_mode = {
    "REG_IMM": 1
}

registers = {
    'rax' : 1,
    'rbx' : 2,
    'rcx' : 3,
    'rdx' : 4,
    'rsi' : 5,
    'rdi' : 6,
    'rsp' : 7,
    'r8' : 8,
    'r9' : 9,
    'r10' : 10,
    'r11' : 11,
    'r12' : 12,
    'r13' : 13,
    'r14' : 14,
    'r15' : 15,
    'rbp' : 16,
    'rip' : 17,
    
    # 8 bit wide registers
    'al' : 18

}


codes = ""
#with open("binsh.myasm", "r") as h:
#with open("pseudocode.myasm", "r") as h:
with open(argv[2], "r") as h:
    codes = h.read()

def parse_data(text):
    data_section = {}
    data_lines = False

    last_arr = []

    for line in text.splitlines():
        line = line.strip()
        # Detect start of .data section
        if line.strip().startswith("section .data"):
            data_lines = True
            continue
        # Detect end of .data section
        elif line.strip().startswith("section"):
            data_lines = False

        if data_lines:
            line = line.strip()
            line = line.split(";")[0]
            print("parsing:", line)
            # Parse labels and values
            match_db = re.match(r'^([a-zA-Z_][a-zA-Z0-9_]*)\s*(db|dw|dd|dq)\s*(.*)$', line)
            match_equ = re.match(r'^([a-zA-Z_][a-zA-Z0-9_]*)\s*(equ)\s*(.*)$', line)

            # Parse db instructions
            if match_db:
                label = match_db.group(1)
                value = match_db.group(3)

                arr = []

                print("found db, label:",label, "value:",value)

                for i in value.split(","):
                    i = i.strip()
                    if i[0] == "\"":
                        arr.extend((ord(o) for o in eval(i)))
                    else:
                        arr.append(int(i))
                
                data_section[label.strip()] = arr

                last_arr = arr


            # Parse equ instructions
            elif match_equ:
                label = match_equ.group(1)
                value = match_equ.group(3)
                print("found equ, label:",label, "value:",value)

                if value[0] == "$":
                    data_section[label.strip()] = len(last_arr)
                else:
                    raise Exception("unimplemented")

    return data_section

def parse_bss(text):
    data_section = {}
    data_lines = False

    for line in text.splitlines():
        line = line.strip()
        # Detect start of .data section
        if line.strip().startswith("section .bss"):
            data_lines = True
            continue
        # Detect end of .data section
        elif line.strip().startswith("section"):
            data_lines = False

        if data_lines:
            line = line.strip()
            line = line.split(";")[0]
            print("parsing:", line)
            # Parse labels and values
            match_resb = re.match(r'^([a-zA-Z_][a-zA-Z0-9_]*)\s*(resb)\s*(.*)$', line)

            # Parse db instructions
            if match_resb:
                label = match_resb.group(1)
                value = match_resb.group(3)
                print("found resb, label:",label, "value:",value)

                data_section[label.strip()] = [0] * eval(value)

    return data_section


data_section = parse_data(codes)
bss_section = parse_bss(codes)
print(data_section, bss_section)

def parse_data_bss(data: dict, bss: dict):
    combined_table = {}
    lhs_mapping = {}
    
    # Process `data` dictionary
    for counter, (lhs, rhs) in enumerate(data.items(), start=1):
        value = (counter << 4) | 1  # Upper bits for counter, lower 4 bits set to 1 for `data` table
        combined_table[value] = rhs
        lhs_mapping[lhs] = value
    
    # Process `bss` dictionary
    for counter, (lhs, rhs) in enumerate(bss.items(), start=1):
        value = (counter << 4) | 0  # Upper bits for counter, lower 4 bits set to 0 for `bss` table
        combined_table[value] = rhs
        lhs_mapping[lhs] = value
    
    return combined_table, lhs_mapping


# lower 4 bit indicates which section is it referring to
# the rest refers to the index
combined_table, label_mapping = parse_data_bss(data_section, bss_section)

print(combined_table, label_mapping)

def convert_to_int(input_str):
    if input_str.strip().startswith("0x"):
        return int(input_str, 16)
    else:
        try:
            return int(input_str, 10)
        except ValueError:
            return None
        
def resolve_offs(text):
    mode, val = 0,0
    if text.strip() in registers.keys():
        mode = 1
        val = registers[text]
    elif text.strip() in label_mapping.keys():
        mode = 2
        val = label_mapping[text]
    else:
        mode = 3
        val = convert_to_int(text)
        if val == None:
            raise Exception("unsupported")
    return val << 2 | mode
        
def resolve_ptr(text):
    # Step 1: Regular expression to match the structure of the pointer
    pattern = r'(?:(byte|word|dword|qword)\s*(?:ptr\s*)?)?\[([a-z0-9_]+)([+-])?([a-z0-9_]+)?\]'
    match = re.match(pattern, text, re.IGNORECASE)
    
    if not match:
        raise ValueError("Invalid pointer format")
    
    # Extract components from regex groups
    size_prefix, base_reg, sign, offset = match.groups()

    print("base_reg",base_reg)
    print("sign", sign)
    print("offset", offset)

    # There are 2 cases for lhs and rhs, imm, label, or register
    lhs = resolve_offs(base_reg.strip())
    rhs = resolve_offs(offset.strip())
    sign_bit = 0 if sign == "+" else 1

    result = (lhs << 11) | (sign_bit << 10) | rhs

    return result
        
def resolve_mov(ops, operands) -> list[int, int, int]:
    lhs = convert_to_int(operands[0])
    rhs = convert_to_int(operands[1])
    #REG_REG
    if operands[0] in registers.keys() and operands[1] in registers.keys():
        return mov_mode["REG_REG"], registers[operands[0]], registers[operands[1]]
    #REG_IMM
    elif operands[0] in registers.keys() and rhs != None:
        return mov_mode["REG_IMM"], registers[operands[0]], rhs
    #REG_PTR
    elif operands[0] in registers.keys() and "[" in operands[1]:
        return mov_mode["REG_PTR"], registers[operands[0]], resolve_ptr(operands[1])
    #REG_LAB
    elif operands[0] in registers.keys():
        return mov_mode["REG_LAB"], registers[operands[0]], label_mapping[operands[1]]
    #PTR_IMM
    elif "[" in operands[0] and rhs != None:
        return mov_mode["PTR_IMM"], resolve_ptr(operands[0]), rhs
    #PTR_REG
    elif "[" in operands[0] and operands[1] in registers.keys():
        return mov_mode["PTR_REG"], resolve_ptr(operands[0]), registers[operands[1]]
    else:
        print("Unsupported mov instructions")
        exit(1)

def resolve_xor(ops, operands) -> list[int, int, int]:
    #REG_REG
    if operands[0] in registers.keys() and operands[1] in registers.keys():
        return xor_mode["REG_REG"], registers[operands[0]], registers[operands[1]]
    #REG_PTR
    elif operands[0] in registers.keys() and "[" in operands[1]:
        return xor_mode["REG_PTR"], registers[operands[0]], resolve_ptr(operands[1])
    #PTR_REG
    elif "[" in operands[0] and operands[1] in registers.keys():
        return xor_mode["PTR_REG"], resolve_ptr(operands[0]), registers[operands[1]]
    else:
        print("Unsupported xor instructions")
        exit(1)

def resolve_ror(ops, operands) -> list[int, int, int]:
    #REG_IMM
    if operands[0] in registers.keys() and rhs != None:
        return ror_mode["REG_IMM"], registers[operands[0]], operands[1]
    #PTR_IMM
    elif "[" in operands[0] and rhs != None: 
        return ror_mode["PTR_IMM"], resolve_ptr(operands[0]), operands[1]
    else:
        print("Unsupported ror instructions")
        exit(1)

def resolve_cmp(ops, operands) -> list[int, int, int]:
    #REG_IMM
    if operands[0] in registers.keys() and rhs != None:
        return cmp_mode["REG_IMM"], registers[operands[0]], rhs
    else:
        print("Unsupported cmp instructions")
        exit(1)

        

#populate label first
pc = 0
label = dict()
text_lines = False
for i in codes.split("\n"):
    # Detect start of .data section
    if i.strip().startswith("section .text"):
        text_lines = True
        continue
    # Detect end of .data section
    elif i.strip().startswith("section"):
        text_lines = False

    if not text_lines:
        continue

    i = i.strip()
    #print(pc, i)

    #comments or blank line
    if len(i) < 1 or i[0] == ";":
        continue

    #labels
    if ":" in i:
        label[i.split(":")[0].strip()] = pc
        continue
    
    #commands
    pc += 1



#compile
pc = 0
code = []
text_lines = False
for i in codes.split("\n"):
    # Detect start of .data section
    if i.strip().startswith("section .text"):
        text_lines = True
        continue
    # Detect end of .data section
    elif i.strip().startswith("section"):
        text_lines = False

    if text_lines:
        i = i.strip()


        print(pc, i)

        #comments or blank line
        if len(i) < 1 or i[0] == ";":
            continue

        #labels
        if ":" in i:
            continue

        #trim the comments
        i = i.split("; ")[0].strip()

        #truncate the operands
        ops = i.split(" ")[0]
        operands = " ".join(i.split(" ")[1:])
        if len(operands.strip()) > 0:
            operands = operands.split(",")
        for i in range(len(operands)):
            operands[i] = operands[i].strip()
        
        print(f"working on opcode {ops} with operands {operands}")

        # ops mode, left hand side, right hand side in nasm
        opm,lhs,rhs = -1,0,0

        #two operands
        if ops_dict[ops] < ops_dict["TWO_OPERANDS"]:
            if ops == "mov":
                opm, lhs, rhs = resolve_mov(ops, operands)
            if ops == "xor":
                opm, lhs, rhs = resolve_xor(ops, operands)
            if ops == "ror":
                opm, lhs, rhs = resolve_ror(ops, operands)
            if ops == "cmp":
                opm, lhs, rhs = resolve_cmp(ops, operands)

        #one operands
        elif ops_dict[ops] < ops_dict["ONE_OPERANDS"]:
            if ops in {"jne", "jmp", "call", "loop"}:
                opm, lhs = 0, label[operands[0]]
            if ops in {"push", "pop", "dec", "inc"}:
                num = convert_to_int(operands[0])
                #Registers
                if num == None:
                    opm, lhs = 0, registers[operands[0]]
                else:
                    opm, lhs = 0, num
            if ops == "repe":
                opm = 0

        #no operands
        elif ops_dict[ops] < ops_dict["NO_OPERANDS"]:
            opm = 0

        if opm == -1:
            print("UNSUPPORTED OPS")
            exit(1)

        code.extend((ops_dict[ops], opm, lhs, rhs))
        
        #commands
        pc += 1

print("code:", code)
print("buffers:", combined_table)
#print("labels:")

print(label)

# with open("program.h" if argv[1] == "1" else "else.h", "w") as h:
#     writeto = f"int progsize = {len(code)};\n"

#     writeto += "const char prog[] = {"
#     for i in code:
#         writeto += str(i) + ","
#     writeto += "};"

#     h.write(writeto)


with open("program.json" if argv[1] == "1" else "else.json", "w") as outfile:
    json.dump({"code":code, "buf":combined_table}, outfile, indent=4)

