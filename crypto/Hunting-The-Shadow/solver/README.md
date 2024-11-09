# Hunting the shadow

> Category: crypto
> Suggested Points: ?

# Description
> Detailed description as it would need to be explained to security engineers

The goal of this challenge is to find the flag hidden in one of the three provided files using a badly secured password. In order to succeed you will need to use all provided files and retrieve that password.


# Flag
`csawctf{D0n't_us3_ECB_m0d3!_1t_can_r3v3al_patt3rns_ab0ut_th3_pla1nt3xt}`

# Solution
> As detailed as possible description of the solution. Not just the solver script. As full a description as possible of the solution for the challenge.

Todo as a challenger:
1. Open ciphertext.txt, identify patterns. 
  - Link the patterns to ECB mode
2. Combine the header.txt and body.txt together to create the complete encrypted image
3. Display the encrypted image 
  - Identify the HP Wolf Security logo and therefore that the blurred out text is HP WOLF SECURITY -> That's the needed password
4. Use HP WOLF SECURITY as a password 
  - Use the key_gen.py file
  - The key_gen.py script tells the user the password is 16 characters exactly  
  - input: password, output: key for decryption (9fc2ea6a6f138002748ff0cae7c4fe36)
5. Decrypt the body using AES-ECB with the key
6. Identify the flag
  - Merge the header and plaintext together to confirm the key has been correctly applied by displaying the file as an image again
  - Locate the flag in the image at the bottom left corner