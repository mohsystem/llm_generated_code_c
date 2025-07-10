#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#define KEY_SIZE 32  // 256-bit AES key

void generate_key(unsigned char *key) {
    if (!RAND_bytes(key, KEY_SIZE)) {
        fprintf(stderr, "Error generating random key\n");
        exit(1);
    }
}

void encrypt_message(const unsigned char *message, int message_len, const unsigned char *key, unsigned char *encrypted) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, KEY_SIZE * 8, &aes_key);

    int padded_len = ((message_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char padded_message[padded_len];
    memcpy(padded_message, message, message_len);
    memset(padded_message + message_len, 0, padded_len - message_len);

    for (int i = 0; i < padded_len; i += AES_BLOCK_SIZE) {
        AES_encrypt(padded_message + i, encrypted + i, &aes_key);
    }
}

void decrypt_message(const unsigned char *encrypted, int encrypted_len, const unsigned char *key, unsigned char *decrypted) {
    AES_KEY aes_key;
    AES_set_decrypt_key(key, KEY_SIZE * 8, &aes_key);

    for (int i = 0; i < encrypted_len; i += AES_BLOCK_SIZE) {
        AES_decrypt(encrypted + i, decrypted + i, &aes_key);
    }
}

int main() {
    unsigned char key[KEY_SIZE];
    generate_key(key);  // Generate key in memory, no file

    const char *message = "Hello, World!";
    int message_len = strlen(message);
    int padded_len = ((message_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    unsigned char encrypted[padded_len];
    unsigned char decrypted[padded_len];

    encrypt_message((unsigned char *)message, message_len, key, encrypted);
    decrypt_message(encrypted, padded_len, key, decrypted);

    printf("Original: %s\n", message);

    printf("Encrypted: ");
    for (int i = 0; i < padded_len; i++) {
        printf("%02x", encrypted[i]);
    }
    printf("\n");

    printf("Decrypted: %s\n", decrypted);

    if (memcmp(message, decrypted, message_len) == 0) {
        printf("Result: PASS\n");
    } else {
        printf("Result: FAIL\n");
    }

    return 0;
}
