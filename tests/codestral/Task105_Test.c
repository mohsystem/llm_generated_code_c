#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>

void initialize_openssl() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}

void cleanup_openssl() {
    EVP_cleanup();
}

int create_ssl_connection(const char* host, const char* port) {
    initialize_openssl();

    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        fprintf(stderr, "Unable to create SSL context\n");
        ERR_print_errors_fp(stderr);
        cleanup_openssl();
        return 1;
    }

    // تحميل شهادات CA الافتراضية
    if (!SSL_CTX_load_verify_locations(ctx, "/etc/ssl/certs/ca-certificates.crt", NULL)) {
        fprintf(stderr, "Failed to load CA certificates\n");
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        cleanup_openssl();
        return 1;
    }

    // الحصول على معلومات الخادم
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv = getaddrinfo(host, port, &hints, &servinfo);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        SSL_CTX_free(ctx);
        cleanup_openssl();
        return 1;
    }

    // إنشاء سوكيت والاتصال بالخادم
    int sockfd = -1;
    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            sockfd = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);

    if (sockfd == -1) {
        fprintf(stderr, "Failed to connect to %s:%s\n", host, port);
        SSL_CTX_free(ctx);
        cleanup_openssl();
        return 1;
    }

    // إنشاء كائن SSL وضبط fd
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    // إجراء مصافحة SSL
    if (SSL_connect(ssl) != 1) {
        fprintf(stderr, "SSL handshake failed\n");
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        cleanup_openssl();
        return 1;
    }

    // تحقق من الشهادة
    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        fprintf(stderr, "Certificate verification error\n");
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        cleanup_openssl();
        return 1;
    }

    printf("Connected to %s:%s\n", host, port);
    printf("SSL version: %s\n", SSL_get_version(ssl));
    printf("Cipher: %s\n", SSL_get_cipher(ssl));

    // تنظيف
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    cleanup_openssl();

    return 0;
}

void run_tests() {
    printf("Running test cases...\n");

    int pass_count = 0;
    int total = 3;

    printf("Test 1 (valid connection): ");
    if (create_ssl_connection("www.google.com", "443") == 0) {
        printf("PASS\n");
        pass_count++;
    } else {
        printf("FAIL\n");
    }

    printf("Test 2 (invalid host): ");
    if (create_ssl_connection("invalid.host.test", "443") != 0) {
        printf("PASS\n");
        pass_count++;
    } else {
        printf("FAIL\n");
    }

    printf("Test 3 (invalid port): ");
    if (create_ssl_connection("www.google.com", "81") != 0) {
        printf("PASS\n");
        pass_count++;
    } else {
        printf("FAIL\n");
    }

    printf("\nSummary: %d/%d tests passed.\n", pass_count, total);
}

int main(int argc, char **argv) {
    if (argc == 3) {
        int ret = create_ssl_connection(argv[1], argv[2]);
        return ret;
    } else {
        printf("Usage: %s <host> <port>\n", argv[0]);
        run_tests();
        return 1;
    }
}
