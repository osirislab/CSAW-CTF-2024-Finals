import argparse
import hashlib
import sys

def derive_key_from_password(password):
	'''
	Do not modify the constants in this function.
	This will break key derivation.
	'''
	password = password.encode('utf-8')
	salt = bytes.fromhex('3cdfb4bf8ada240a2b26e72b5c4f9699')
	iterations = 100000
	key_length = 16
	key = hashlib.pbkdf2_hmac('sha256', password, salt, iterations, key_length)
	return key.hex()

def main():
    parser = argparse.ArgumentParser(description="PBKDF2 Encryption Key Generator")
    
    parser.add_argument("password", help="The password to derive the encryption key from")
    
    args = parser.parse_args()
    provided_password = args.password

    if len(provided_password) != 16:
        print("Error: Password must be exactly 16 characters long.")
        sys.exit(0)

    key = derive_key_from_password(provided_password)
    print("Derived Encryption Key: {}".format(key))

if __name__ == "__main__":
    main()



