#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MAX_DNS_SIZE 512
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 53

#pragma comment(lib, "ws2_32.lib")
#include <stdint.h>  // Add this line for uint16_t and uint8_t

//fixed error by adding #include <stdint.h> , error: unknown type name 'uint8_t'
struct dns_header {
    uint16_t  id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

struct dns_question {
    char* qname;
    uint16_t qtype;
    uint16_t qclass;
};

void parse_dns_query(uint8_t* buffer, struct dns_header* header, struct dns_question* question) {
    memcpy(header, buffer, sizeof(struct dns_header));
    header->id = ntohs(header->id);
    header->flags = ntohs(header->flags);
    header->qdcount = ntohs(header->qdcount);
    header->ancount = ntohs(header->ancount);
    header->nscount = ntohs(header->nscount);
    header->arcount = ntohs(header->arcount);

    int offset = sizeof(struct dns_header);
    int name_length = 0;

    // The rest of the function remains the same
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr));

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("DNS server listening on %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        uint8_t buffer[MAX_DNS_SIZE];

        int recv_len = recvfrom(sock, (char*)buffer, MAX_DNS_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (recv_len == SOCKET_ERROR) {
            fprintf(stderr, "recvfrom failed\n");
            continue;
        }

        struct dns_header header;
        struct dns_question question;
        parse_dns_query(buffer, &header, &question);

        // Process the DNS query and send a response
        // (This part would be implemented based on your specific requirements)
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}