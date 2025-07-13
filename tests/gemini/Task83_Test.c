#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

void test_case_1() {
    // Input key
    unsigned char key[32];
    const char* key_str = "This is a secret key";
    int key_len = strlen(key_str);
    memcpy(key, key_str, key_len);

    // Input data to encrypt
    unsigned char plaintext[1024] = "This is a secret message";
    int plaintext_len = strlen((char*)plaintext);

    // Generate a random salt
    unsigned char salt[16];
    if (!RAND_bytes(salt, sizeof(salt))) {
        printf("Test case 1: fail (RAND_bytes salt failed)\n");
        return;
    }

    // Derive a 256-bit key from the input key and salt
    const EVP_MD* kdf = EVP_sha256();
    unsigned char derived_key[32];
    if (!PKCS5_PBKDF2_HMAC(key_str, key_len, salt, sizeof(salt), 390000, kdf, sizeof(derived_key), derived_key)) {
        printf("Test case 1: fail (PBKDF2 failed)\n");
        return;
    }

    // Generate a random initialization vector (IV)
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        printf("Test case 1: fail (RAND_bytes iv failed)\n");
        return;
    }

    // Encrypt the plaintext using AES-CBC
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        printf("Test case 1: fail (EVP_CIPHER_CTX_new failed)\n");
        return;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, derived_key, iv) != 1) {
        printf("Test case 1: fail (EVP_EncryptInit_ex failed)\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    int len;
    int ciphertext_len = 0;
    unsigned char ciphertext[1024];
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1) {
        printf("Test case 1: fail (EVP_EncryptUpdate failed)\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
        printf("Test case 1: fail (EVP_EncryptFinal_ex failed)\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    if (ciphertext_len > 0) {
        printf("Test case 1: pass\n");
        printf("Ciphertext (hex): ");
        for (int i = 0; i < ciphertext_len; i++) {
            printf("%02x", ciphertext[i]);
        }
        printf("\n");
    } else {
        printf("Test case 1: fail (empty ciphertext)\n");
    }
}

int main() {
    test_case_1();
    return 0;
}
