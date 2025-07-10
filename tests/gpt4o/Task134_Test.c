#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

void save_key(RSA* rsa, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file for writing");
        exit(1);
    }
    PEM_write_RSAPrivateKey(file, rsa, NULL, NULL, 0, NULL, NULL);
    fclose(file);
}

RSA* load_key(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Unable to open file for reading");
        exit(1);
    }
    RSA* rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL);
    fclose(file);
    if (!rsa) {
        fprintf(stderr, "Failed to load RSA private key\n");
        exit(1);
    }
    return rsa;
}

int main() {
    // Generate RSA keypair
    RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);

    // Save private key to file
    save_key(rsa, "private_key.pem");

    // Load private key back from file
    RSA* loaded_rsa = load_key("private_key.pem");

    // Access RSA p component
    const BIGNUM *p = NULL;
    const BIGNUM *q = NULL;
    RSA_get0_factors(loaded_rsa, &p, &q);

    if (p == NULL) {
        printf("Failed to get RSA p component\n");
        RSA_free(rsa);
        RSA_free(loaded_rsa);
        return 1;
    }

    // Convert p to hex string
    char *key_str = BN_bn2hex(p);
    printf("RSA p component: %s\n", key_str);

    // Simple pass/fail: check key_str is non-empty
    if (key_str && strlen(key_str) > 0) {
        printf("Result: PASS\n");
    } else {
        printf("Result: FAIL\n");
    }

    OPENSSL_free(key_str);
    RSA_free(rsa);
    RSA_free(loaded_rsa);

    return 0;
}
