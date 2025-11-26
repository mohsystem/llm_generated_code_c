#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

// دالة حساب هاش الشهادة بصيغة DER وSHA256
void get_cert_hash(const char *cert_path, char *output) {
    FILE *file = fopen(cert_path, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file\n");
        exit(EXIT_FAILURE);
    }

    X509 *cert = PEM_read_X509(file, NULL, NULL, NULL);
    fclose(file);

    if (!cert) {
        fprintf(stderr, "Unable to read certificate\n");
        exit(EXIT_FAILURE);
    }

    int len = i2d_X509(cert, NULL);
    unsigned char *der = malloc(len);
    unsigned char *p = der;
    i2d_X509(cert, &p);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(der, len, hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[SHA256_DIGEST_LENGTH * 2] = '\0';

    X509_free(cert);
    free(der);
}

// دالة التحقق مع طباعة Pass/Fail
void check_certificate_hash(const char *cert_path, const char *known_hash) {
    char cert_hash[SHA256_DIGEST_LENGTH * 2 + 1];
    get_cert_hash(cert_path, cert_hash);

    if (strcmp(cert_hash, known_hash) == 0) {
        printf("Test check_certificate_hash: PASS\n");
    } else {
        printf("Test check_certificate_hash: FAIL\n");
        printf("Calculated hash: %s\n", cert_hash);
        printf("Known hash:      %s\n", known_hash);
    }
}

int main() {
    // عدل المسار ومسار الشهادة إلى مكان شهادة لديك
    const char *cert_path = "/home/kali/google_cert.pem";

    // الهاش المعروف لشهادة google.com (من الخطوة السابقة)
    const char *known_hash = "bdda50ec92bd745e266b7ba3d157184dfa9328234eb5b5b38e116b16d966fd7e";

    check_certificate_hash(cert_path, known_hash);
    return 0;
}
