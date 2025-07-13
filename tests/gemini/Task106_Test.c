#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

int main() {
    // Generate RSA key pair
    RSA *rsa = RSA_new();
    BIGNUM *bne = BN_new();
    BN_set_word(bne, RSA_F4);
    RSA_generate_key_ex(rsa, 2048, bne, NULL);
    BN_free(bne);

    // Test cases (same exact from python)
    const char* test_cases[] = {
        "",
        "Hello",
        "RSA Test 123!"
    };

    for (int t = 0; t < 3; t++) {
        const char* message = test_cases[t];
        int message_length = strlen(message);
        int encrypted_length = RSA_size(rsa);
        unsigned char *encrypted = malloc(encrypted_length);
        unsigned char *decrypted = malloc(encrypted_length);

        int length = RSA_public_encrypt(message_length, (const unsigned char *)message, encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
        if (length == -1) {
            fprintf(stderr, "Test case %d: Encryption error: %s\n", t+1, ERR_error_string(ERR_get_error(), NULL));
            free(encrypted);
            free(decrypted);
            continue;
        }

        int decrypted_length = RSA_private_decrypt(length, encrypted, decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
        if (decrypted_length == -1) {
            fprintf(stderr, "Test case %d: Decryption error: %s\n", t+1, ERR_error_string(ERR_get_error(), NULL));
            free(encrypted);
            free(decrypted);
            continue;
        }

        decrypted[decrypted_length] = '\0';

        if (strcmp(message, (char*)decrypted) == 0) {
            printf("Test case %d: PASS\n", t + 1);
        } else {
            printf("Test case %d: FAIL\n", t + 1);
            printf("Original: '%s'\n", message);
            printf("Decrypted: '%s'\n", decrypted);
        }

        free(encrypted);
        free(decrypted);
    }

    RSA_free(rsa);
    return 0;
}
