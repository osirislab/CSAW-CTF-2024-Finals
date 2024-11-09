# Custom authentication scheme:
>Category: Crypto
>Suggested points: ?

# Description
>We've used a flag to create our very own unbreakable authenticated encryption scheme. Try out our oracle!

# Flag
csawctf{d0n'T_r0Il_cu$t0m_A3AD}

# What the user gets:
>query.py

# Solution:
> Todo as a challenger:
> 1. Test the authenticated encryption oracle with various input messages.
> 2. Send the same 16-byte block to the authenticated oracle in different block positions. Notice that the outputs are the same, so ECB is used in the oracle. 
> 3. Send messages of different lengths as well as an empty string. Notice that the length of the output increases and is > than the length of the input message.
> 4. Send different messages of length 16 bytes. Notice that the last 32 bytes returned by the oracle are the same. 
> 5. Send different messages of another length, e.g. 15 bytes. Notice that the last 32 bytes returned by the oracle are again the same. But they are different from step 4: the oracle appends a secret value to the message before encrypting it. (This is the flag)
> 6. Notice that the oracle does the following: for an input message m, it computes ECB(m||flag||pad).
> 7. Use the oracle to recover the secret value. To recover the secret value you can do a chosen plaintext attack: https://crypto.stackexchange.com/questions/42891/chosen-plaintext-attack-on-aes-in-ecb-mode
> 8. The recovered secret value is "csawctf{d0n'T_r0Il_cu$t0m_A3AD}", it's the flag and it's 31 bytes long. 
> The script to solve this is in solver.py