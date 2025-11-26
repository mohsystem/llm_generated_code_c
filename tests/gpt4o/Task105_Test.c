#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int create_ssl_connection(const char *hostname, int port) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "Error creating SSL context\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // تحميل شهادات CA الافتراضية
    if (!SSL_CTX_load_verify_locations(ctx, "/etc/ssl/certs/ca-certificates.crt", NULL)) {
        fprintf(stderr, "Failed to load CA certificates\n");
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        SSL_CTX_free(ctx);
        return 1;
    }

    struct hostent *host = gethostbyname(hostname);
    if (!host) {
        fprintf(stderr, "Error resolving hostname %s\n", hostname);
        close(sockfd);
        SSL_CTX_free(ctx);
        return 1;
    }

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    memcpy(&dest_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    if (connect(sockfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) != 0) {
        perror("connect");
        close(sockfd);
        SSL_CTX_free(ctx);
        return 1;
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) <= 0) {
        fprintf(stderr, "SSL connection failed\n");
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        return 1;
    }

    // تحقق من الشهادة
    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        fprintf(stderr, "Certificate verification error\n");
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        return 1;
    }

    printf("Connected to %s:%d\n", hostname, port);
    printf("Cipher: %s\n", SSL_get_cipher(ssl));
    printf("SSL version: %s\n", SSL_get_version(ssl));

    SSL_free(ssl);
    close(sockfd);
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
        printf("Usage: %s <hostname> <port>\n", argv[0]);
        run_tests();
        return 1;
    }
}
