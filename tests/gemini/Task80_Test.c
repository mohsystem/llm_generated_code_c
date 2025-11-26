#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>       // for if_nametoindex
#include <openssl/ssl.h>
#include <openssl/err.h>

// دالة إنشاء SSL IPv6 socket مع دعم zone index (واجهة الشبكة)
int create_ssl_client_socket_ipv6(const char *hostname, int port) {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        return -1;
    }

    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "Failed to create SSL context\n");
        close(sockfd);
        return -1;
    }

    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        fprintf(stderr, "Failed to create SSL object\n");
        SSL_CTX_free(ctx);
        close(sockfd);
        return -1;
    }

    if (SSL_set_fd(ssl, sockfd) <= 0) {
        fprintf(stderr, "Failed to bind SSL object to socket\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sockfd);
        return -1;
    }

    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(port);

    // نسخ العنوان لفصل zone index
    char host_copy[256];
    strncpy(host_copy, hostname, sizeof(host_copy));
    host_copy[sizeof(host_copy)-1] = '\0';

    char *percent = strchr(host_copy, '%');
    unsigned int scope_id = 0;
    if (percent) {
        *percent = '\0';  // فصل العنوان عن اسم الواجهة
        scope_id = if_nametoindex(percent + 1);
        if (scope_id == 0) {
            fprintf(stderr, "Invalid interface name in IPv6 address\n");
            SSL_free(ssl);
            SSL_CTX_free(ctx);
            close(sockfd);
            return -1;
        }
    }

    if (inet_pton(AF_INET6, host_copy, &server_addr.sin6_addr) <= 0) {
        perror("Invalid address/Address not supported");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sockfd);
        return -1;
    }

    server_addr.sin6_scope_id = scope_id;

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to server");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sockfd);
        return -1;
    }

    if (SSL_connect(ssl) <= 0) {
        fprintf(stderr, "Failed to establish SSL connection\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sockfd);
        return -1;
    }

    return sockfd;
}

// دالة التست لطباعة Pass/Fail
void run_test(const char* host, int port) {
    int sockfd = create_ssl_client_socket_ipv6(host, port);
    if (sockfd >= 0) {
        printf("Test create_ssl_ipv6_socket: PASS\n");
        close(sockfd);
    } else {
        printf("Test create_ssl_ipv6_socket: FAIL\n");
    }
}

int main() {
    // جرب عنوان IPv6 محلي مع واجهة eth0
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