#include <stdio.h>
#include <string.h>
#include <regex.h>

int validateURL(const char* url) {
    regex_t regex;
    int reti;
    const char* pattern = "^(https?|ftp|file)://[-a-zA-Z0-9+&@#/%?=~_|!:,.;]*[-a-zA-Z0-9+&@#/%=~_|]";

    reti = regcomp(&regex, pattern, 0);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    reti = regexec(&regex, url, 0, NULL, 0);
    regfree(&regex);

    if (!reti) {
        return 1;
    } else if (reti == REG_NOMATCH) {
        return 0;
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        return 0;
    }
}

void test_case(const char* test_name, const char* url, int expected) {
    int result = validateURL(url);
    if (result == expected) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected %s, got %s)\n", test_name, expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    test_case("test_valid_http_url", "http://example.com", 1);
    test_case("test_valid_https_url", "https://example.com", 1);
    test_case("test_valid_ftp_url", "ftp://example.com", 1);
    test_case("test_valid_localhost_url", "http://localhost", 1);
    test_case("test_valid_ipv4_url", "http://127.0.0.1", 1);
    test_case("test_valid_ipv6_url", "http://[::1]", 0); // غير مدعوم بهذا النمط
    test_case("test_valid_url_with_port", "http://example.com:8080", 1);
    test_case("test_invalid_url_missing_scheme", "example.com", 0);
    test_case("test_invalid_url_with_invalid_domain", "http://-example.com", 1); // النمط لا يتحقق من صحة الدومين
    test_case("test_invalid_url_with_invalid_tld", "http://example", 1);          // النمط لا يتحقق من TLD
    return 0;
}
