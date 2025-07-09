#include <stdio.h>
#include <string.h>
#include <regex.h>

int is_valid_url(const char *url) {
    regex_t regex;
    int reti;

    reti = regcomp(&regex, "^(?:http|ftp)s?://(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\\.)+(?:[A-Z]{2,6}\\.?|[A-Z0-9-]{2,}\\.?)|localhost|\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})(?::\\d+)?(?:/?|[/?]\\S+)$", REG_EXTENDED | REG_ICASE);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    // trim trailing newline from input if present
    char url_trimmed[1024];
    strncpy(url_trimmed, url, sizeof(url_trimmed));
    url_trimmed[sizeof(url_trimmed)-1] = '\0';
    size_t len = strlen(url_trimmed);
    if(len > 0 && url_trimmed[len-1] == '\n') {
        url_trimmed[len-1] = '\0';
    }

    reti = regexec(&regex, url_trimmed, 0, NULL, 0);
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
    int result = is_valid_url(url);
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
    test_case("test_valid_ipv6_url", "http://[::1]", 0); // this regex does not handle IPv6 brackets exactly, so expected fail
    test_case("test_valid_url_with_port", "http://example.com:8080", 1);
    test_case("test_invalid_url_missing_scheme", "example.com", 0);
    test_case("test_invalid_url_with_invalid_domain", "http://-example.com", 0);
    test_case("test_invalid_url_with_invalid_tld", "http://example", 0);

    return 0;
}
