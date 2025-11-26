#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

RSA* generateKey() {
    RSA *rsa = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, RSA_F4);
    if (!RSA_generate_key_ex(rsa, 2048, e, NULL)) {
        fprintf(stderr, "RSA_generate_key_ex failed\n");
        exit(1);
    }
    BN_free(e);
    return rsa;
}

void saveKey(RSA* rsa, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("File open failed");
        exit(1);
    }
    if (!PEM_write_RSAPrivateKey(fp, rsa, NULL, NULL, 0, NULL, NULL)) {
        fprintf(stderr, "PEM_write_RSAPrivateKey failed\n");
        exit(1);
    }
    fclose(fp);
}

RSA* loadKey(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("File open failed");
        exit(1);
    }
    RSA* rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!rsa) {
        fprintf(stderr, "PEM_read_RSAPrivateKey failed\n");
        exit(1);
    }
    return rsa;
}

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

int main() {
    const char* keyFile = "private.pem";

    RSA* rsa = NULL;
    FILE* f = fopen(keyFile, "rb");
    if (!f) {
        // Key file doesn't exist, generate new key and save it
        rsa = generateKey();
        saveKey(rsa, keyFile);
    } else {
        fclose(f);
        rsa = loadKey(keyFile);
    }

    const char* message = "Hello, World!";
    unsigned char encrypted[256]; // 2048 bits = 256 bytes max ciphertext
    unsigned char decrypted[256];
    int encrypted_length, decrypted_length;

    // Encrypt with public key
    encrypted_length = RSA_public_encrypt(strlen(message), (unsigned char*)message,
                                          encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    if (encrypted_length == -1) {
        handleErrors();
    }

    // Decrypt with private key
    decrypted_length = RSA_private_decrypt(encrypted_length, encrypted,
                                           decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    if (decrypted_length == -1) {
        handleErrors();
    }
    decrypted[decrypted_length] = '\0'; // Null-terminate decrypted string

    printf("Original: %s\n", message);

    printf("Encrypted: ");
    for (int i = 0; i < encrypted_length; i++) {
        printf("%02x", encrypted[i]);
    }
    printf("\n");

    printf("Decrypted: %s\n", decrypted);

    // Check and print pass/fail
    if (strcmp(message, (char*)decrypted) == 0) {
        printf("Result: PASS\n");
    } else {
        printf("Result: FAIL\n");
    }

    RSA_free(rsa);

    return 0;
}
