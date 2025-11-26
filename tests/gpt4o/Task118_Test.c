#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

// دالة التحقق من صحة IP (كما في كودك)
bool validateIP(const char *ip) {
    regex_t regex;
    const char *pattern = "^([0-9]{1,3}\\.){3}[0-9]{1,3}$";
    if (regcomp(&regex, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return false;
    }
    if (regexec(&regex, ip, 0, NULL, 0) != 0) {
        regfree(&regex);
        return false;
    }
    regfree(&regex);

    char ipCopy[16];
    strcpy(ipCopy, ip);
    char *token = strtok(ipCopy, ".");
    int octet_count = 0;
    while (token != NULL) {
        int num = atoi(token);
        if (num < 0 || num > 255) {
            return false;
        }
        octet_count++;
        token = strtok(NULL, ".");
    }
    if (octet_count != 4) return false;
    return true;
}

// دالة تصنيف IP أو إعادة "Invalid IP address"
const char* process_ip(const char* ip) {
    if (!validateIP(ip)) {
        return "Invalid IP address";
    }
    char ipCopy[16];
    strcpy(ipCopy, ip);
    char *firstOctetStr = strtok(ipCopy, ".");
    if (!firstOctetStr) return "Invalid IP address";
    int firstOctet = atoi(firstOctetStr);

    if (firstOctet >= 1 && firstOctet <= 126) return "A";
    else if (firstOctet >= 128 && firstOctet <= 191) return "B";
    else if (firstOctet >= 192 && firstOctet <= 223) return "C";
    else if (firstOctet >= 224 && firstOctet <= 239) return "D";
    else if (firstOctet >= 240 && firstOctet <= 255) return "E";
    else return "Invalid IP address";
}

// دالة لعمل تيست وطباعه pass/fail
void run_test(const char* testName, const char* ip, const char* expected) {
    const char* result = process_ip(ip);
    printf("Test: %s\nInput: %s\nExpected: %s\nGot: %s\n", testName, ip, expected, result);
    if (strcmp(result, expected) == 0) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }
}

int main() {
    run_test("valid_class_a_ip", "10.0.0.1", "A");
    run_test("valid_class_b_ip", "172.16.0.1", "B");
    run_test("valid_class_c_ip", "192.168.0.1", "C");
    run_test("valid_class_d_ip", "224.0.0.1", "D");
    run_test("valid_class_e_ip", "240.0.0.1", "E");

    run_test("invalid_ip_with_non_numeric_octets", "192.abc.0.1", "Invalid IP address");
    run_test("invalid_ip_with_out_of_range_octets", "256.0.0.1", "Invalid IP address");
    run_test("invalid_ip_with_empty_octets", "192.168..1", "Invalid IP address");
    run_test("invalid_ip_with_missing_octets", "192.168.0", "Invalid IP address");
    run_test("invalid_ip_with_extra_octets", "192.168.0.1.1", "Invalid IP address");

    return 0;
}
