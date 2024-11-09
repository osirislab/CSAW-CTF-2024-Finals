from Crypto.Cipher import AES
import hashlib
import os
import secrets
import random
'''
This solver.py script will allow you to create the CTF and solve it. 

The first part of this script will take a clear bitmap image of the HP Wolf Security logo 
and encrypt it using AES-ECB. The encryption key comes from the output of a PBKDF2 function 
which requires the input of the password 'HP WOLF SECURITY'. The encrypted body of the bitmap
image is stored as bytes to a file called 'body.txt' alongside the bitmap header as 'header.txt' 
which is what is given to the competitors.

*As a verifier this script will load the bitmap header from the 'header.txt' and then append it to 
the bytes of the 'body.txt' and save it as `encrypted_flag.bmp` so that it can be viewed in an image 
viewer to see the scrambled image*

The competitors will need to identify the patterns in the bytes of the 'body.txt' to first learn that 
it is encrypted in ECB mode and secondly figure out it is an image. The competitors will need to
identify that the 'header.txt' is a Bitmap image header from the first 2 bytes 'BM' which is the
signature for a BMP image. They will need to append the 'header.txt' to the 'body.txt' to view the
scrambled image.They will need to infer the password from this scrambled image to derive the correct
decryption key.

The second part of this script will use the key to AES-ECB decrypt the ciphertext using the key derived from
the password 'HP WOLF SECURITY' and append the bitmap header then store the file as the decrypted_flag.bmp. 
This image can now be viewed in an image viewer where it will no longer look distorted, but will present the 
clear image of the logo. The flag is visible in the bottom left corner of the image which is what the challengers
are expected to locate!
'''

#BITMAP_HEADER = b'BMVB\x16\x00\x00\x00\x00\x006\x00\x00\x00(\x00\x00\x00\xf5\x03\x00\x00\xe0\x01\x00\x00\x01\x00\x18\x00\x00\x00\x00\x00 B\x16\x00\xc4\x0e\x00\x00\xc4\x0e\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'


with open('header.txt', 'rb') as bitmap_file:
	BITMAP_HEADER = bitmap_file.read()

def derive_key_from_password(password):
	password = password.encode('utf-8')
	salt = bytes.fromhex('3cdfb4bf8ada240a2b26e72b5c4f9699')
	iterations = 100000
	key_length = 16
	key = hashlib.pbkdf2_hmac('sha256', password, salt, iterations, key_length)
	return key.hex()

def encrypt_and_store_image_ciphertext(plain_flag, password, output):
	key = derive_key_from_password(
		password=password
	)

	print("Encryption key used for encryption: {}".format(key))

	cipher = AES.new(bytes.fromhex(key), AES.MODE_ECB)
	cipher_text = cipher.encrypt(plain_flag)
	print("\n")
	print("First 1000 bytes of encrypted bitmap body:")
	print(cipher_text[:4000].hex())

	# saves just the ciphertext body to the ciphertext.txt
	with open(output, 'wb') as writer:
		writer.write(cipher_text)
		writer.close()

	# saves the ciphertext appended with the bitmap header to view as an image
	with open('encrypted_flag.bmp', 'wb') as writer:
		writer.write(BITMAP_HEADER + cipher_text)
		writer.close()

	return key

def decrypt_and_store_image(encrypted_flag_file, password, output):
	key = derive_key_from_password(
		password=password
	)
	print("\n")
	print("Encryption key used for decryption: {}".format(key))

	cipher = AES.new(bytes.fromhex(key), AES.MODE_ECB)

	with open(encrypted_flag_file, 'rb') as image_data:
		encrypted_flag_image = image_data.read()
		plaintext = cipher.decrypt(encrypted_flag_image)

	with open(output, 'wb') as writer:
		writer.write(BITMAP_HEADER + plaintext)
		writer.close()

	return None


with open('original_image.bmp', 'rb') as file:
	image_data = file.read()
	image_body = image_data[54:]

encrypt_and_store_image_ciphertext(
	plain_flag=image_body,
	password='HP WOLF SECURITY',
	output='body.txt'
)

decrypt_and_store_image(
	encrypted_flag_file='body.txt',
	password='HP WOLF SECURITY',
	output='decrypted_flag.bmp'
)