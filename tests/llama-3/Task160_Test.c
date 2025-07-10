#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    strcat((char*)userp, (char*)contents);
    return size * nmemb;
}

int test_url(const char* url, const char* expected_substring) {
    char readBuffer[10240];
    readBuffer[0] = '\0';  // مهم جداً لتهيئة السلسلة فارغة

    CURL *curl;
    CURLcode res;
    long response_code = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(!curl) {
        curl_global_cleanup();
        return 0; // فشل تهيئة curl
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);
    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // الحصول على كود الاستجابة
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    // نتحقق إذا النص المتوقع موجود في المحتوى
    int pass = 0;
    if(expected_substring == NULL) {
        pass = (strlen(readBuffer) == 0);
    } else {
        pass = (strstr(readBuffer, expected_substring) != NULL);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return pass;
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

    int n = sizeof(tests)/sizeof(tests[0]);
    for(int i = 0; i < n; i++) {
        printf("Test %d (%s): ", i+1, tests[i].description);
        int pass = test_url(tests[i].url, tests[i].expected_substring);
        printf("%s\n", pass ? "PASS" : "FAIL");
    }
}

int main() {
    run_tests();

    char url[256];
    printf("\nEnter a URL: ");
    fgets(url, 256, stdin);
    url[strcspn(url, "\n")] = 0;

    char readBuffer[10240];
    readBuffer[0] = '\0';

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        } else {
            long response_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            printf("Status Code: %ld\n", response_code);
            printf("Content: %s\n", readBuffer);
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}
