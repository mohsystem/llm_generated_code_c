#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// دالة للتحقق من صحة شكل الـ IP (0-255 لكل أوكتيت، 4 أوكتيت)
int validateIP(const char* ip) {
    regex_t regex;
    int reti;

    // regex للتحقق من الشكل (أرقام 1-3 لكل أوكتيت مفصولة بنقاط)
    // لاحظ: \\ في السلسلة C لتصبح \ في regex
    reti = regcomp(&regex, "^([0-9]{1,3}\\.){3}[0-9]{1,3}$", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    reti = regexec(&regex, ip, 0, NULL, 0);
    if (reti == REG_NOMATCH) {
        regfree(&regex);
        return 0;
    } else if (reti != 0) {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        regfree(&regex);
        exit(1);
    }

    regfree(&regex);

    // تحقق من نطاق كل أوكتيت (0-255)
    char ipCopy[20];
    strncpy(ipCopy, ip, sizeof(ipCopy));
    ipCopy[sizeof(ipCopy)-1] = '\0';

    char *token = strtok(ipCopy, ".");
    int count = 0;
    while (token != NULL) {
        int val = atoi(token);
        if (val < 0 || val > 255) {
            return 0;
        }
        count++;
        token = strtok(NULL, ".");
    }
    if (count != 4) return 0;

    return 1;
}

// دالة process_ip كما في البايثون: ترجع اسم الكلاس أو "Invalid IP address"
const char* process_ip(const char* ip) {
    if (!validateIP(ip)) {
        return "Invalid IP address";
    }

    // ناخذ الأوكتيت الأول فقط
    char ipCopy[20];
    strncpy(ipCopy, ip, sizeof(ipCopy));
    ipCopy[sizeof(ipCopy)-1] = '\0';

    char* firstOctetStr = strtok(ipCopy, ".");
    if (!firstOctetStr) {
        return "Invalid IP address";
    }
    int firstOctet = atoi(firstOctetStr);

    if (firstOctet >= 1 && firstOctet <= 126) {
        return "A";
    } else if (firstOctet >= 128 && firstOctet <= 191) {
        return "B";
    } else if (firstOctet >= 192 && firstOctet <= 223) {
        return "C";
    } else if (firstOctet >= 224 && firstOctet <= 239) {
        return "D";
    } else if (firstOctet >= 240 && firstOctet <= 255) {
        return "E";
    } else {
        return "Invalid IP address";
    }
}

// دالة لفحص نتائج التيست كيسز مع طباعة pass/fail
void run_test(const char* testName, const char* ip, const char* expected) {
    const char* result = process_ip(ip);
    printf("Test: %s\n", testName);
    printf("Input IP: %s\n", ip);
    printf("Expected: %s, Got: %s\n", expected, result);
    if (strcmp(result, expected) == 0) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }
}

int main() {
    // نفس التيست كيسز بالترتيب والنفس القيم
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
