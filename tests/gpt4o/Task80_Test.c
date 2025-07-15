#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>      // for if_nametoindex
#include <openssl/ssl.h>
#include <openssl/err.h>

// دالة إنشاء SSL IPv6 socket مع دعم zone index (واجهة الشبكة)
int create_ssl_client_socket(const char *hostname, int port) {
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Unable to create socket");
        SSL_CTX_free(ctx);
        return -1;
    }

    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);

    // نسخ العنوان لفصل zone index
    char host_copy[256];
    strncpy(host_copy, hostname, sizeof(host_copy));
    host_copy[sizeof(host_copy)-1] = '\0';

    char *percent = strchr(host_copy, '%');
    if (percent) {
        *percent = '\0';
        unsigned int scope_id = if_nametoindex(percent + 1);
        if (scope_id == 0) {
            fprintf(stderr, "Invalid interface name\n");
            SSL_CTX_free(ctx);
            close(sock);
            return -1;
        }
        addr.sin6_scope_id = scope_id;
    } else {
        addr.sin6_scope_id = 0;
    }

    if (inet_pton(AF_INET6, host_copy, &addr.sin6_addr) <= 0) {
        perror("inet_pton");
        SSL_CTX_free(ctx);
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connect");
        SSL_CTX_free(ctx);
        close(sock);
        return -1;
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sock);
        return -1;
    }

    return sock;  // إرجاع ملف الوصف (socket fd)
}

// دالة اختبار لطباعة Pass أو Fail
void run_test(const char* host, int port) {
    int sockfd = create_ssl_client_socket(host, port);
    if (sockfd >= 0) {
        printf("Test create_ssl_client_socket: PASS\n");
        close(sockfd);
    } else {
        printf("Test create_ssl_client_socket: FAIL\n");
    }
}

int main() {
    // اختبار باستخدام عنوان IPv6 محلي مع zone index
    run_test("fe80::a02c:661e:cd7e:aee5%eth0", 443);

    return 0;
}
//many steps to run this code.
/*
 * creating a Temp OpenSSL Certificate => openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
 * Combine Key and Certificate => cat key.pem cert.pem > server.pem
 * Run the Server => sudo openssl s_server -accept 443 -www -cert server.pem -key server.pem
 * Then Run the code
 */