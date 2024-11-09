import json
import requests
import hashlib
 
url = "http://127.0.0.1:5121/enc/oracle"

def call_oracle(data):
    return requests.post(url=url, json=data).json()["ciphertext"]

data = {"plaintext": ""}
first_ciphertext = call_oracle(data) 

print("Sending an empty string to the oracle. Size of the returned ciphertext:", len(first_ciphertext))
#We hypothesise the oracle returns ECB_key(message||secret||padding)

print("Starting the chosen plaintext attack ...")
#We need to chose a known plaintext of length: len(first_ciphertext)-1.
#We then need to brute force the unknown message of the encryption oracle using his known plaintext
#We'll perform a chosen plaintext attack: https://crypto.stackexchange.com/questions/42891/chosen-plaintext-attack-on-aes-in-ecb-mode

#This is our first known plaintext=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA 

result=""
for i in range(len(first_ciphertext)):
    if i == len(first_ciphertext)-1:
        known_plain=""
    else:
        known_plain = (len(first_ciphertext)-1-i)*'A'
    print("Sending known plaintext: ", known_plain)
    data = {"plaintext": known_plain}
    to_attack = call_oracle(data)[0:len(first_ciphertext)]
    
    #Here we brute-force the unknown character from the secret passphrase.
    for j in range(32,127):
        data = {"plaintext": known_plain + result+ chr(j)}
        test_cipher = call_oracle(data)[0:len(first_ciphertext)]
    
        if (to_attack==test_cipher):
            print("Recovered character: ", chr(j))
            result=result+chr(j)
            break
#Result is the cracked unknown plaintext.
print("Reached the end of attack.")
print("Unknown secret flag, now recovered: ", result)


