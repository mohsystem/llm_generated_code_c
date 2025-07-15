#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// الدالة كما هي (بدون تعديل)
SSL* createSSLClientSocket(const char* host, int port) {
    SSL_library_init();
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    SSL* ssl = SSL_new(ctx);
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    inet_pton(AF_INET6, host, &addr.sin6_addr);
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    SSL_set_fd(ssl, sockfd);
    SSL_connect(ssl);
    return ssl;
}

// دالة اختبار تطبع Pass/Fail
void run_test(const char* host, int port) {
    SSL* ssl = createSSLClientSocket(host, port);
    if (ssl != NULL) {
        printf("Test createSSLClientSocket: PASS\n");
        SSL_shutdown(ssl);
        SSL_free(ssl);
    } else {
        printf("Test createSSLClientSocket: FAIL\n");
    }
}

int main() {
    // مثال اختبار (عدل العنوان والمنفذ حسب بيئتك)
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