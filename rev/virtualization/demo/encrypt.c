#include <stdio.h>

unsigned long long encrypt(unsigned long long number) {
    unsigned long long encrypted = number;
    
    // Encryption routine using bit shifts and XORs with constants
    encrypted ^= 0xA5A5A5A5A5A5A5A5ULL;  // XOR with a 64-bit constant
    encrypted = (encrypted << 5) | (encrypted >> (64 - 5));  // Left circular shift by 5 bits
    encrypted ^= 0x5A5A5A5A5A5A5A5AULL;  // XOR with another 64-bit constant
    
    return encrypted;
}

unsigned long long decrypt(unsigned long long encrypted) {
    unsigned long long decrypted = encrypted;
    
    // Reverse the encryption steps in the opposite order
    decrypted ^= 0x5A5A5A5A5A5A5A5AULL;  // XOR with the same constant as in encryption
    decrypted = (decrypted >> 5) | (decrypted << (64 - 5));  // Right circular shift by 5 bits
    decrypted ^= 0xA5A5A5A5A5A5A5A5ULL;  // XOR with the initial constant
    
    return decrypted;
}

int main() {
    unsigned long long numbers[3] = {
	    0x7b66746377617363,
	    0x315f4e314c54304b,
	    0x007d59344b305f53
    };  // Original number to encrypt

    
    for(int i = 0; i < 3; i++)
    {
 
    	unsigned long long encrypted = encrypt(numbers[i]);
    unsigned long long decrypted = decrypt(encrypted);
   printf("Original: %llu\n", numbers[i]);
    printf("Encrypted: %llu\n", encrypted);
    printf("Decrypted: %llu\n\n", decrypted);
    }
    return 0;
}

