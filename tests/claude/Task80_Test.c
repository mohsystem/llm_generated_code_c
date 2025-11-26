#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// الدالة كما هي بالضبط من طلبك:
SSL* create_ssl_ipv6_socket(const char* host, int port) {
    int sock;
    struct addrinfo hints, *res, *p;
    char port_str[6];
    SSL_CTX *ctx;
    SSL *ssl;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;

    snprintf(port_str, sizeof(port_str), "%d", port);

    if (getaddrinfo(host, port_str, &hints, &res) != 0) {
        perror("getaddrinfo");
        return NULL;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == -1) continue;

        if (connect(sock, p->ai_addr, p->ai_addrlen) != -1) break;

        close(sock);
    }

    freeaddrinfo(res);

    if (p == NULL) {
        fprintf(stderr, "Failed to connect\n");
        return NULL;
    }

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLS_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    if (SSL_connect(ssl) == -1) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sock);
        return NULL;
    }

    return ssl;
}

// دالة اختبار الحالة (test case)
void run_test(const char* host, int port) {
    SSL *ssl = create_ssl_ipv6_socket(host, port);
    if (ssl != NULL) {
        printf("Test create_ssl_ipv6_socket: PASS\n");
        SSL_shutdown(ssl);
        SSL_free(ssl);
    } else {
        printf("Test create_ssl_ipv6_socket: FAIL\n");
    }
}

int main() {
    // من التست في البايثون:
    // create_ssl_ipv6_socket("192.168.1.233", 443)

    run_test("fe80::a02c:661e:cd7e:aee5%eth0",
             443);

    return 0;
}


//many steps to run this code.
/*
 * creating a Temp OpenSSL Certificate => openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
 * Combine Key and Certificate => cat key.pem cert.pem > server.pem
 * Run the Server => sudo openssl s_server -accept 443 -www -cert server.pem -key server.pem
 * Then Run the code
 */