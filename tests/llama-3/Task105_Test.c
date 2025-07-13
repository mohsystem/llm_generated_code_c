#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

int create_ssl_connection(const char *host, int port) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "Error creating SSL context\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }

    char conn_str[256];
    snprintf(conn_str, sizeof(conn_str), "%s:%d", host, port);

    BIO *bio = BIO_new_connect(conn_str);
    if (!bio) {
        fprintf(stderr, "Error creating connection BIO\n");
        SSL_CTX_free(ctx);
        return 1;
    }

    if (BIO_do_connect(bio) <= 0) {
        fprintf(stderr, "Error connecting to server\n");
        ERR_print_errors_fp(stderr);
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 1;
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_bio(ssl, bio, bio);

    if (SSL_connect(ssl) <= 0) {
        fprintf(stderr, "SSL handshake failed\n");
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 1;
    }

    printf("SSL connection established to %s:%d\n", host, port);

    const char *request = "Hello, world!";
    if (SSL_write(ssl, request, strlen(request)) <= 0) {
        fprintf(stderr, "SSL write failed\n");
        ERR_print_errors_fp(stderr);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 1;
    }

    char buffer[1024] = {0};
    int bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        printf("Received: %s\n", buffer);
    } else {
        fprintf(stderr, "SSL read failed or connection closed\n");
        ERR_print_errors_fp(stderr);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 1;
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
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
    if (argc == 3) {
        int ret = create_ssl_connection(argv[1], atoi(argv[2]));
        return ret;
    } else {
        printf("Usage: %s <host> <port>\n", argv[0]);
        run_tests();
        return 1;
    }
}
