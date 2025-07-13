#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

void encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
             unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    int len;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    int ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

// Test case 1 مطابق للكود الأساسي
void test_case_1() {
    unsigned char key[16];
    if (!RAND_bytes(key, sizeof(key))) {
        printf("Test case 1: fail (RAND_bytes key failed)\n");
        return;
    }

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        printf("Test case 1: fail (RAND_bytes iv failed)\n");
        return;
    }

    unsigned char *plaintext = (unsigned char *)"exampleplaintext";
    int plaintext_len = strlen((char *)plaintext);

    unsigned char ciphertext[128];
    memset(ciphertext, 0, sizeof(ciphertext));

    encrypt(plaintext, plaintext_len, key, iv, ciphertext);

    // نفترض أن النص المشفر طوله على الأقل طول النص مدعوم بالبلوك (أي 16 أو أكثر)
    // لأننا لا نعرف الطول بالضبط (encrypt لا تعيد الطول)،
    // لكن يمكننا التحقق أن أول 16 بايت على الأقل ليست كلها صفر

    int non_zero_bytes = 0;
    for (int i = 0; i < 16; i++) {
        if (ciphertext[i] != 0) non_zero_bytes++;
    }

    if (non_zero_bytes > 0) {
        printf("Test case 1: pass\n");
        printf("Ciphertext (first 16 bytes hex): ");
        for (int i = 0; i < 16; i++) {
            printf("%02x", ciphertext[i]);
        }
        printf("\n");
    } else {
        printf("Test case 1: fail (ciphertext all zero)\n");
    }
}

int main(void) {
    test_case_1();
    return 0;
}
