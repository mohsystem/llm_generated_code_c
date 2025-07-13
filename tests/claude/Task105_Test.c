#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void initialize_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl() {
    EVP_cleanup();
}

SSL_CTX* create_context() {
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        fprintf(stderr, "Unable to create SSL context\n");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    // تحميل شهادات CA الافتراضية
    if (!SSL_CTX_load_verify_locations(ctx, "/etc/ssl/certs/ca-certificates.crt", NULL)) {
        fprintf(stderr, "Failed to load CA certificates\n");
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

int create_ssl_connection(const char* host, int port) {
    SSL_CTX* ctx = create_context();
    SSL* ssl = SSL_new(ctx);

    BIO* bio = BIO_new_ssl_connect(ctx);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    char connection_string[256];
    snprintf(connection_string, sizeof(connection_string), "%s:%d", host, port);
    BIO_set_conn_hostname(bio, connection_string);

    if (BIO_do_connect(bio) <= 0) {
        fprintf(stderr, "Error connecting to server\n");
        ERR_print_errors_fp(stderr);
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

    BIO_get_ssl(bio, &ssl);

    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        fprintf(stderr, "Certificate verification error\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 1;
    }

    printf("Connected to %s:%d\n", host, port);
    printf("SSL version: %s\n", SSL_get_version(ssl));
    printf("Cipher: %s\n", SSL_get_cipher(ssl));

    BIO_free_all(bio);
    SSL_CTX_free(ctx);
    return 0;
}

void run_tests() {
    printf("Running test cases...\n");

    int pass_count = 0;
    int total = 3;

    printf("Test 1 (valid connection): ");
    if (create_ssl_connection("www.google.com", 443) == 0) {
        printf("PASS\n");
        pass_count++;
    } else {
        printf("FAIL\n");
    }

    printf("Test 2 (invalid host): ");
    if (create_ssl_connection("invalid.host.test", 443) != 0) {
        printf("PASS\n");
        pass_count++;
    } else {
        printf("FAIL\n");
    }

    printf("Test 3 (invalid port): ");
    if (create_ssl_connection("www.google.com", 81) != 0) {
        printf("PASS\n");
        pass_count++;
    } else {
        printf("FAIL\n");
    }

    printf("\nSummary: %d/%d tests passed.\n", pass_count, total);
}

int main(int argc, char **argv) {
    initialize_openssl();

    if (argc == 3) {
        int ret = create_ssl_connection(argv[1], atoi(argv[2]));
        cleanup_openssl();
        return ret;
    } else {
        printf("Usage: %s <host> <port>\n", argv[0]);
        run_tests();
        cleanup_openssl();
        return 1;
    }
}
