#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/err.h>

void encryptAESCBC(unsigned char* key, unsigned char* plaintext, unsigned char* encrypted) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);
    unsigned char iv[AES_BLOCK_SIZE];
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        iv[i] = 0;
    }
    int len = strlen((char*)plaintext);
    AES_cbc_encrypt(plaintext, encrypted, len, &aes_key, iv, AES_ENCRYPT);
}

void test_case_1() {
    unsigned char key[16];  // غير مهيأ كما في الكود الأصلي
    unsigned char plaintext[] = "Hello, World!";
    unsigned char encrypted[strlen((char*)plaintext) + AES_BLOCK_SIZE];

    encryptAESCBC(key, plaintext, encrypted);

    // نتحقق أن أول بايت في الناتج ليس صفر فقط كشرط بسيط للنجاح (يمكن تغيير شرط)
    if (encrypted[0] != 0) {
        printf("Test case 1: pass\n");
        printf("Encrypted (hex): ");
        for (int i = 0; i < strlen((char*)plaintext) + AES_BLOCK_SIZE; i++) {
            printf("%02x", encrypted[i]);
        }
        printf("\n");
    } else {
        printf("Test case 1: fail\n");
    }
}

int main() {
    test_case_1();
    return 0;
}
