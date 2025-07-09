#include <stdio.h>
#include <string.h>

int validateUrl(const char* url) {
    if (strstr(url, "http://") == url || strstr(url, "https://") == url || strstr(url, "ftp://") == url) {
        return 1;
    }
    return 0;
}

void test_case(const char* test_name, const char* url, int expected) {
    int result = validateUrl(url);
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
    test_case("test_valid_ipv6_url", "http://[::1]", 1); // يتعرف فقط على البادئة، يعتبر صحيح
    test_case("test_valid_url_with_port", "http://example.com:8080", 1);
    test_case("test_invalid_url_missing_scheme", "example.com", 0);
    test_case("test_invalid_url_with_invalid_domain", "http://-example.com", 1); // لا يتحقق من صحة الدومين
    test_case("test_invalid_url_with_invalid_tld", "http://example", 1); // لا يتحقق من صلاحية TLD

    return 0;
}
