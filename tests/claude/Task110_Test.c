#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>

#define MAX_URL_LENGTH 2048

// دالة التحقق من صحة URL بناءً على regex مماثل للبايثون
bool isValidURL(const char* url) {
    regex_t regex;
    int reti;

    // Regex لتطابق URL مع البروتوكولات http, https, ftp فقط
    // لاحظ أن هذا التعبير بسيط ولن يغطي كل تفاصيل URL بشكل كامل لكنه يتبع المطلوب
    const char* pattern = "^(https?|ftp)://([^\\s/$.?#].)*$";

    reti = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return false;
    }

    reti = regexec(&regex, url, 0, NULL, 0);
    regfree(&regex);

    return (reti == 0);
}

// دالة لطباعة نتيجة التست case مع pass/fail
void test_case(const char* test_name, const char* url, bool expected) {
    bool result = isValidURL(url);
    if (result == expected) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected %s, got %s)\n", test_name, expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    // تكرار التست كيسز الموجودة في البايثون بدون تغيير

    test_case("test_valid_http_url", "http://example.com", true);
    test_case("test_valid_https_url", "https://example.com", true);
    test_case("test_valid_ftp_url", "ftp://example.com", true);
    test_case("test_valid_localhost_url", "http://localhost", true);
    test_case("test_valid_ipv4_url", "http://127.0.0.1", true);
    test_case("test_valid_ipv6_url", "http://[::1]", true);
    test_case("test_valid_url_with_port", "http://example.com:8080", true);
    test_case("test_invalid_url_missing_scheme", "example.com", false);
    test_case("test_invalid_url_with_invalid_domain", "http://-example.com", false);
    test_case("test_invalid_url_with_invalid_tld", "http://example", false);

    return 0;
}
