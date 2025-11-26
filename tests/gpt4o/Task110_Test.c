#include <stdio.h>
#include <stdbool.h>
#include <regex.h>
#include <string.h>

bool validateURL(const char *url) {
    regex_t regex;
    const char *pattern = "^(http|https|ftp)://([^\\s/$.?#].[^\\s]*)$";
    if (regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE) != 0)
        return false;

    int reti = regexec(&regex, url, 0, NULL, 0);
    regfree(&regex);
    return reti == 0;
}

void test_case(const char* test_name, const char* url, bool expected) {
    bool result = validateURL(url);
    if (result == expected) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected %s, got %s)\n", test_name, expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    test_case("test_valid_http_url", "http://example.com", true);
    test_case("test_valid_https_url", "https://example.com", true);
    test_case("test_valid_ftp_url", "ftp://example.com", true);
    test_case("test_valid_localhost_url", "http://localhost", true);
    test_case("test_valid_ipv4_url", "http://127.0.0.1", true);
    test_case("test_valid_ipv6_url", "http://[::1]", false); // غير مدعوم في هذا التعبير النمطي
    test_case("test_valid_url_with_port", "http://example.com:8080", true);
    test_case("test_invalid_url_missing_scheme", "example.com", false);
    test_case("test_invalid_url_with_invalid_domain", "http://-example.com", false);
    test_case("test_invalid_url_with_invalid_tld", "http://example", false);

    return 0;
}
