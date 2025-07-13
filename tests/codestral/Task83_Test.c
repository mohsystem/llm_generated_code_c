#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>

void encryptKey(const char* key, const char* cipherKey, unsigned char* encryptedKey, int* encryptedLen) {
    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);
    memcpy(encryptedKey, iv, AES_BLOCK_SIZE);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (const unsigned char*)cipherKey, iv);

    int len = 0, ciphertext_len = 0;
    unsigned char* ciphertext = malloc(strlen(key) + AES_BLOCK_SIZE);
    EVP_EncryptUpdate(ctx, ciphertext, &len, (const unsigned char*)key, strlen(key));
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    memcpy(encryptedKey + AES_BLOCK_SIZE, ciphertext, ciphertext_len);
    *encryptedLen = AES_BLOCK_SIZE + ciphertext_len;

    free(ciphertext);
}

void test_encryptKey_case_1() {
    const char* key = "This is a key123";
    const char* cipherKey = "This is a cipher123";
    unsigned char encryptedKey[1024];
    int encryptedLen = 0;

    encryptKey(key, cipherKey, encryptedKey, &encryptedLen);

    if (encryptedLen > AES_BLOCK_SIZE) {
        printf("Test case 1: pass\n");
        printf("Encrypted key (hex): ");
        for (int i = 0; i < encryptedLen; i++) {
            printf("%02x", encryptedKey[i]);
        }
        printf("\n");
    } else {
        printf("Test case 1: fail\n");
    }
}

int main() {
    test_encryptKey_case_1();
    return 0;
}
