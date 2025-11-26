#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>  // إضافة لتعريف X509

int create_ssl_connection(const char* host, const char* port) {
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "Error creating SSL context\n");
        return 1;
    }

    BIO *bio = BIO_new_ssl_connect(ctx);
    if (!bio) {
        fprintf(stderr, "Error creating BIO\n");
        SSL_CTX_free(ctx);
        return 1;
    }

    char conn_str[256];
    snprintf(conn_str, sizeof(conn_str), "%s:%s", host, port);
    BIO_set_conn_hostname(bio, conn_str);

    SSL *ssl = NULL;
    BIO_get_ssl(bio, &ssl);
    if (!ssl) {
        fprintf(stderr, "Can't locate SSL pointer\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 1;
    }
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    if (BIO_do_connect(bio) <= 0) {
        fprintf(stderr, "Error establishing connection\n");
        ERR_print_errors_fp(stderr);
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 1;
    }

    if (BIO_do_handshake(bio) <= 0) {
        fprintf(stderr, "Error establishing SSL handshake\n");
        ERR_print_errors_fp(stderr);
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 1;
    }

    // التحقق من الشهادة
    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        fprintf(stderr, "Certificate verification error\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 1;
    }

    X509* cert = SSL_get_peer_certificate(ssl);
    if (cert) {
        char* subj = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);
        if (subj) {
            printf("Connected to %s:%s\nCertificate Subject: %s\n", host, port, subj);
            OPENSSL_free(subj);
        }
        X509_free(cert);
    } else {
        printf("No certificate received\n");
    }

    printf("SSL/TLS version: %s\n", SSL_get_version(ssl));
    printf("Cipher suite: %s\n", SSL_get_cipher(ssl));

    BIO_free_all(bio);
    SSL_CTX_free(ctx);

    return 0;
}

void run_tests() {
    printf("Running test cases...\n");

    int pass = 0, total = 3;

    printf("Test 1 (valid connection): ");
    if (create_ssl_connection("www.google.com", "443") == 0) {
        printf("PASS\n");
        pass++;
    } else {
        printf("FAIL\n");
    }

    printf("Test 2 (invalid host): ");
    if (create_ssl_connection("invalid.host.test", "443") != 0) {
        printf("PASS\n");
        pass++;
    } else {
        printf("FAIL\n");
    }

    printf("Test 3 (invalid port): ");
    if (create_ssl_connection("www.google.com", "81") != 0) {
        printf("PASS\n");
        pass++;
    } else {
        printf("FAIL\n");
    }

    printf("\nSummary: %d/%d tests passed.\n", pass, total);
}

int main(int argc, char** argv) {
    if (argc == 3) {
        int ret = create_ssl_connection(argv[1], argv[2]);
        return ret;
    } else {
        printf("Usage: %s <host> <port>\n", argv[0]);
        run_tests();
        return 1;
    }
}
