#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int bits = 2048;
    unsigned long e = RSA_F4;
    RSA *rsa = RSA_generate_key(bits, e, NULL, NULL);

    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(pri, rsa, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, rsa);

    size_t pri_len = BIO_pending(pri);
    size_t pub_len = BIO_pending(pub);

    char *pri_key = (char*)malloc(pri_len + 1);
    char *pub_key = (char*)malloc(pub_len + 1);

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    // printf("Public Key: \n%s\n", pub_key);
    // printf("Private Key: \n%s\n", pri_key);

    // نفس التست كيسز
    const char* test_cases[] = {
        "",
        "Hello",
        "RSA Test 123!"
    };

    for (int t = 0; t < 3; t++) {
        const char* message = test_cases[t];
        RSA *rsa_public = RSA_new();
        BIO *pubbio = BIO_new_mem_buf(pub_key, -1);
        PEM_read_bio_RSAPublicKey(pubbio, &rsa_public, NULL, NULL);

        int encrypted_length = RSA_size(rsa_public);
        unsigned char *encrypted = (unsigned char*)malloc(encrypted_length);

        int enc_len = RSA_public_encrypt(strlen(message), (const unsigned char*)message, encrypted, rsa_public, RSA_PKCS1_OAEP_PADDING);
        if(enc_len == -1) {
            char err[130];
            ERR_load_crypto_strings();
            ERR_error_string(ERR_get_error(), err);
            printf("Test case %d: Encryption error: %s\n", t+1, err);
            BIO_free_all(pubbio);
            RSA_free(rsa_public);
            free(encrypted);
            continue;
        }

        RSA *rsa_private = RSA_new();
        BIO *pribio = BIO_new_mem_buf(pri_key, -1);
        PEM_read_bio_RSAPrivateKey(pribio, &rsa_private, NULL, NULL);

        unsigned char *decrypted = (unsigned char*)malloc(enc_len + 1);
        int dec_len = RSA_private_decrypt(enc_len, encrypted, decrypted, rsa_private, RSA_PKCS1_OAEP_PADDING);

        if(dec_len == -1) {
            char err[130];
            ERR_load_crypto_strings();
            ERR_error_string(ERR_get_error(), err);
            printf("Test case %d: Decryption error: %s\n", t+1, err);
            BIO_free_all(pribio);
            RSA_free(rsa_private);
            BIO_free_all(pubbio);
            RSA_free(rsa_public);
            free(encrypted);
            free(decrypted);
            continue;
        }

        decrypted[dec_len] = '\0';

        if (strcmp(message, (char*)decrypted) == 0) {
            printf("Test case %d: PASS\n", t + 1);
        } else {
            printf("Test case %d: FAIL\n", t + 1);
            printf("Original: '%s'\n", message);
            printf("Decrypted: '%s'\n", decrypted);
        }

        BIO_free_all(pribio);
        RSA_free(rsa_private);
        BIO_free_all(pubbio);
        RSA_free(rsa_public);
        free(encrypted);
        free(decrypted);
    }

    RSA_free(rsa);
    BIO_free_all(pri);
    BIO_free_all(pub);
    free(pri_key);
    free(pub_key);

    return 0;
}
