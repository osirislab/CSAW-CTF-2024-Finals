def read_and_process_file(filename):
    try:
        # Read and clean content from the file
        with open(filename, 'r') as file:
            content = file.read().strip()

        # Convert each character to its ASCII code
        ascii_values = [ord(char) for char in content]

        # Ensure length is a multiple of 8 for complete 64-bit groups, pad if necessary
        if len(ascii_values) % 8 != 0:
            ascii_values.extend([0] * (8 - len(ascii_values) % 8))

        # Group into sets of 8 and convert to little-endian 64-bit numbers
        little_endian_64bit_numbers = []
        for i in range(0, len(ascii_values), 8):
            group = ascii_values[i:i+8]
            # Convert group to little-endian 64-bit integer
            little_endian_64bit_number = sum(byte << (8 * index) for index, byte in enumerate(group))
            little_endian_64bit_numbers.append(little_endian_64bit_number)

        return little_endian_64bit_numbers

    except FileNotFoundError:
        print(f"Error: {filename} not found.")
        return None

# Usage
filename = 'flag.txt'
result = read_and_process_file(filename)
if result is not None:
    print("Little-endian 64-bit numbers:")
    for num in result:
        print(str(num) + ", " + "0x" + hex(num)[2:].zfill(16))

