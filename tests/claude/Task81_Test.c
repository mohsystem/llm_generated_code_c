#include <stdio.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#define MAX_HASH_LENGTH 65

// دالة استدعاء شهادة SSL وحساب SHA256 لها
int get_certificate_hash(const char* hostname, int port, char* hash_out) {
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "Failed to create SSL context\n");
        return -1;
    }

    BIO* bio = BIO_new_ssl_connect(ctx);
    if (!bio) {
        fprintf(stderr, "Failed to create BIO object\n");
        SSL_CTX_free(ctx);
        return -1;
    }

    char conn_str[256];
    snprintf(conn_str, sizeof(conn_str), "%s:%d", hostname, port);
    BIO_set_conn_hostname(bio, conn_str);

    if (BIO_do_connect(bio) <= 0) {
        fprintf(stderr, "Failed to connect\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return -1;
    }

    if (BIO_do_handshake(bio) <= 0) {
        fprintf(stderr, "Failed to perform SSL handshake\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return -1;
    }

    SSL *ssl = NULL;
    BIO_get_ssl(bio, &ssl);
    if (!ssl) {
        fprintf(stderr, "Can't locate SSL pointer\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return -1;
    }

    X509* cert = SSL_get_peer_certificate(ssl);
    if (!cert) {
        fprintf(stderr, "Failed to get peer certificate\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return -1;
    }

    unsigned char md[SHA256_DIGEST_LENGTH];
    unsigned int md_len = 0;
    if (!X509_digest(cert, EVP_sha256(), md, &md_len)) {
        fprintf(stderr, "Failed to compute certificate digest\n");
        X509_free(cert);
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return -1;
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash_out[i * 2], "%02x", md[i]);
    }
    hash_out[SHA256_DIGEST_LENGTH * 2] = '\0';

    X509_free(cert);
    BIO_free_all(bio);
    SSL_CTX_free(ctx);

    return 0;
}

// دالة اختبار صارمة تطبع PASS أو FAIL
void check_certificate_hash(const char* hostname, const char* known_hash) {
    char cert_hash[MAX_HASH_LENGTH];
    int result = get_certificate_hash(hostname, 443, cert_hash);
    if (result == 0) {
        if (strcmp(cert_hash, known_hash) == 0) {
            printf("Test check_certificate_hash for %s: PASS\n", hostname);
        } else {
            printf("Test check_certificate_hash for %s: FAIL\n", hostname);
            printf("Calculated hash: %s\n", cert_hash);
            printf("Known hash:      %s\n", known_hash);
        }
    } else {
        printf("Test check_certificate_hash for %s: FAIL (error getting certificate hash)\n", hostname);
    }
}

int main() {
    // Test case: google.com with known valid SHA256 hash
    check_certificate_hash("google.com",
        "bdda50ec92bd745e266b7ba3d157184dfa9328234eb5b5b38e116b16d966fd7e");

    return 0;
}
/*
 * Befor running the code, run these commands =>



┌──(kali㉿kali)-[~]
└─$ echo | openssl s_client -connect google.com:443 -servername google.com 2>/dev/null | openssl x509 > google_cert.pem


┌──(kali㉿kali)-[~]
└─$ openssl x509 -in google_cert.pem -noout -fingerprint -sha256

sha256 Fingerprint=BD:DA:50:EC:92:BD:74:5E:26:6B:7B:A3:D1:57:18:4D:FA:93:28:23:4E:B5:B5:B3:8E:11:6B:16:D9:66:FD:7E
 */
