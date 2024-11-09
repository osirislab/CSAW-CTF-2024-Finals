import string
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import hashlib

key=bytes('8g9ik?$gdfjk0]4%', 'ascii')
passphrase=b"csawctf{d0n'T_r0Il_cu$t0m_A3AD}"
            

#encryption_oracle.
#concatenates input m to the private passphrase.
#returns its AES-ECB encryption.
def auth_encrypt_oracle2(m):
    if m=="":
        plaintext = passphrase
    if type(m) is str:
        m = bytes(m, 'ascii')
    cipher = AES.new(key, AES.MODE_ECB)
     #print(pad(message, AES.block_size))
    print(m)
    plaintext = m + passphrase
    ciphertext = cipher.encrypt(pad(plaintext, AES.block_size))
    #print(type(ciphertext))
    return ciphertext





