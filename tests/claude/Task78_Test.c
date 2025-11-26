#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

char* generate_rsa_key() {
    RSA *rsa = RSA_new();
    BIGNUM *bne = BN_new();
    if (!BN_set_word(bne, RSA_F4)) {
        RSA_free(rsa);
        BN_free(bne);
        return NULL;
    }
    if (!RSA_generate_key_ex(rsa, 2048, bne, NULL)) {
        RSA_free(rsa);
        BN_free(bne);
        return NULL;
    }

    BIO *bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
        BIO_free_all(bio);
        RSA_free(rsa);
        BN_free(bne);
        return NULL;
    }

    char *pem_key;
    long pem_size = BIO_get_mem_data(bio, &pem_key);
    char *ssh_key = malloc(pem_size + 1);
    if (!ssh_key) {
        BIO_free_all(bio);
        RSA_free(rsa);
        BN_free(bne);
        return NULL;
    }
    memcpy(ssh_key, pem_key, pem_size);
    ssh_key[pem_size] = '\0';

    BIO_free_all(bio);
    RSA_free(rsa);
    BN_free(bne);

    return ssh_key;
}

int main() {
    // Test case 1: generate RSA key and print it
    char *ssh_key = generate_rsa_key();
    if (ssh_key) {
        printf("Test case 1: PASS\n");
        printf("%s\n", ssh_key);
        free(ssh_key);
    } else {
        printf("Test case 1: FAIL\n");
    }

    return 0;
}
