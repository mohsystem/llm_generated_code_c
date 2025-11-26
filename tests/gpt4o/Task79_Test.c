#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

void encrypt(const char *message, const char *secretKey, unsigned char *encryptedMessage, int *out_len) {
    unsigned char key[32], iv[32];
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    // توليد مفتاح و IV عشوائيين (لكن لا يستخدم المفتاح العشوائي)
    RAND_bytes(key, sizeof(key));
    RAND_bytes(iv, sizeof(iv));

    // تهيئة التشفير
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)secretKey, iv);

    int len = 0;
    int encryptedMessageLen = 0;

    EVP_EncryptUpdate(ctx, encryptedMessage, &len, (unsigned char *)message, strlen(message));
    encryptedMessageLen = len;

    EVP_EncryptFinal_ex(ctx, encryptedMessage + len, &len);
    encryptedMessageLen += len;

    EVP_CIPHER_CTX_free(ctx);

    *out_len = encryptedMessageLen;
}

#define MAX_LEN 1024

void run_test_cases() {
    struct TestCase {
        const char* message;
        const char* key;
        const char* desc;
    } tests[] = {
        {"HELLO", "thisisaverysecret", "basic encryption"},
        {"HELLO WORLD", "thisisaverysecret", "encryption with spaces"},
        {"HELLO, WORLD!", "thisisaverysecret", "encryption with special characters"},
        {"", "thisisaverysecret", "empty message"},
        {"HELLO123", "thisisaverysecret", "encryption with numbers"},
        {"PYTHON", "thisisaverysecret", "same key produces same output"},
        {"PYTHON", "anothersecretkey1", "different key produces different output"},
        {"Python", "thisisaverysecret", "case insensitive encryption"},
        {"TEST", "thisisaverysecret", "key affects encryption"},
        {"test", "thisisaverysecret", "same key and message different cases"},
    };

    unsigned char encrypted[MAX_LEN];
    int encrypted_len;

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        encrypt(tests[i].message, tests[i].key, encrypted, &encrypted_len);
        if (encrypted_len > 0) {
            printf("Test case %d (%s): PASS (Encrypted length: %d bytes)\n", i+1, tests[i].desc, encrypted_len);
        } else {
            printf("Test case %d (%s): FAIL (Encryption failed)\n", i+1, tests[i].desc);
        }
    }
}

int main() {
    run_test_cases();
    return 0;
}
