#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// دالة validate_ip كما هي مع regex دقيق
int validate_ip(const char *ip) {
    regex_t regex;
    int reti;

    const char *pattern = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    reti = regexec(&regex, ip, 0, NULL, 0);

    regfree(&regex);

    if (!reti) {
        return 1;  // true
    } else if (reti == REG_NOMATCH) {
        return 0;  // false
    } else {
        // لا نصل هنا لأن regfree مسبقا
        return 0;
    }
}

// دالة تصنيف IP أو Invalid
const char* process_ip(const char* ip) {
    if (!validate_ip(ip)) {
        return "Invalid IP address";
    }

    char ipCopy[20];
    strncpy(ipCopy, ip, sizeof(ipCopy));
    ipCopy[sizeof(ipCopy)-1] = '\0';

    char* firstOctetStr = strtok(ipCopy, ".");
    if (!firstOctetStr) return "Invalid IP address";

    int firstOctet = atoi(firstOctetStr);

    if (firstOctet >= 1 && firstOctet <= 126) return "A";
    else if (firstOctet >= 128 && firstOctet <= 191) return "B";
    else if (firstOctet >= 192 && firstOctet <= 223) return "C";
    else if (firstOctet >= 224 && firstOctet <= 239) return "D";
    else if (firstOctet >= 240 && firstOctet <= 255) return "E";
    else return "Invalid IP address";
}

// دالة لتشغيل التيست وطباعة pass/fail
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
