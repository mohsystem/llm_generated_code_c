#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((char*)userp) == NULL ? strncpy(((char*)userp), (char*)contents, size * nmemb) : strcat(((char*)userp), (char*)contents);
    return size * nmemb;
}

int test_url(const char* url, const char* expected_substring) {
    char buffer[10000] = {0};

    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) return 0;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return 0;

    if (expected_substring == NULL) {
        // إذا متوقع نص فارغ، هل buffer فارغ؟
        return strlen(buffer) == 0;
    } else {
        return strstr(buffer, expected_substring) != NULL;
    }
}

void run_tests() {
    struct {
        const char* url;
        const char* expected_substring;
        const char* description;
    } tests[] = {
        {"https://www.google.com", "Google", "valid_url_google"},
        {"https://www.bing.com", "Bing", "valid_url_bing"},
        {"https://www.nonexistentwebsite1234567890.com", NULL, "invalid_url"},
        {"https://httpstat.us/500", NULL, "http_error_500"},
        {"https://httpstat.us/302", "Redirect", "redirect"},
        {"https://httpstat.us/200?sleep=5000", NULL, "timeout_or_slow_response"},
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < n; i++) {
        printf("Test %d (%s): ", i+1, tests[i].description);
        int pass = test_url(tests[i].url, tests[i].expected_substring);
        printf("%s\n", pass ? "PASS" : "FAIL");
    }
}

int main() {
    run_tests();

    // ثم الكود التفاعلي كما هو:
    char url[256];
    char buffer[10000] = {0};

    printf("\nEnter a URL: ");
    scanf("%255s", url);

    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    printf("%s\n", buffer);
    return 0;
}
