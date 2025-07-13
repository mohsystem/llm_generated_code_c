#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Generate a new RSA key pair
    RSA* rsa = RSA_new();
    BIGNUM* e = BN_new();
    BN_set_word(e, RSA_F4);
    RSA_generate_key_ex(rsa, 2048, e, NULL);

    // Test cases
    const char* test_cases[] = {
        "",
        "Hello",
        "RSA Test 123!"
    };

    for (int t = 0; t < 3; t++) {
        const char* message = test_cases[t];
        size_t dataLength = strlen(message);
        unsigned char* encryptedData = malloc(RSA_size(rsa));
        unsigned char* decryptedData = malloc(RSA_size(rsa));
        int encrypted_length = RSA_public_encrypt((int)dataLength, (const unsigned char*)message, encryptedData, rsa, RSA_PKCS1_PADDING);

        if (encrypted_length == -1) {
            fprintf(stderr, "Encryption failed for test case %d\n", t+1);
            free(encryptedData);
            free(decryptedData);
            continue;
        }

        int decrypted_length = RSA_private_decrypt(encrypted_length, encryptedData, decryptedData, rsa, RSA_PKCS1_PADDING);
        if (decrypted_length == -1) {
            fprintf(stderr, "Decryption failed for test case %d\n", t+1);
            free(encryptedData);
            free(decryptedData);
            continue;
        }

        decryptedData[decrypted_length] = '\0'; // null-terminate decrypted string

        if (strcmp(message, (char*)decryptedData) == 0) {
            printf("Test case %d: PASS\n", t + 1);
        } else {
            printf("Test case %d: FAIL\n", t + 1);
            printf("Original: '%s'\n", message);
            printf("Decrypted: '%s'\n", decryptedData);
        }

        free(encryptedData);
        free(decryptedData);
    }

    // Clean up
    RSA_free(rsa);
    BN_free(e);

    return 0;
}
