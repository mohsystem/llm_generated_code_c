#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    char** responsePtr = (char**)userp;

    // إعادة تخصيص لتخزين البيانات المتزايدة
    char* ptr = realloc(*responsePtr, totalSize + 1);
    if(ptr == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 0;
    }
    *responsePtr = ptr;

    memcpy(*responsePtr, contents, totalSize);
    (*responsePtr)[totalSize] = '\0';

    return totalSize;
}


// WriteCallback صحيحة تجمع كل البيانات (للتست)
size_t WriteCallbackAccum(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        fprintf(stderr, "Not enough memory\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int test_url(const char* url, const char* expected_substring, int expect_error) {
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    int pass = 0;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackAccum);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl);

        if((res != CURLE_OK) && !expect_error) {
            pass = 0;
        } else if((res == CURLE_OK) && expect_error) {
            pass = 0;
        } else if(expect_error) {
            pass = 1;
        } else {
            if(expected_substring == NULL) {
                pass = (chunk.size == 0);
            } else if(strstr(chunk.memory, expected_substring) != NULL) {
                pass = 1;
            } else {
                pass = 0;
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    free(chunk.memory);

    return pass;
}

void run_tests() {
    struct {
        const char* url;
        const char* expected_substring;
        int expect_error; // 1 = error expected, 0 = success expected
        const char* description;
    } tests[] = {
        {"https://www.google.com", "Google", 0, "valid_url_google"},
        {"https://www.bing.com", "Bing", 0, "valid_url_bing"},
        {"https://www.nonexistentwebsite1234567890.com", NULL, 1, "invalid_url"},
        {"https://httpstat.us/500", NULL, 1, "http_error_500"},
        {"https://httpstat.us/302", "Redirect", 0, "redirect"},
        {"https://httpstat.us/200?sleep=5000", NULL, 1, "timeout_or_slow_response"},
    };

    int n = sizeof(tests)/sizeof(tests[0]);
    for(int i = 0; i < n; i++) {
        printf("Running test %d (%s)... ", i+1, tests[i].description);
        int pass = test_url(tests[i].url, tests[i].expected_substring, tests[i].expect_error);
        printf("%s\n", pass ? "PASS" : "FAIL");
    }
}

int main() {
    // Run tests first
    run_tests();

    // Original code (unchanged)
    char url[1024];
    printf("\nEnter URL: ");
    fgets(url, sizeof(url), stdin);
    url[strcspn(url, "\n")] = 0; // Remove trailing newline

    CURL* curl = curl_easy_init();
    if (curl) {
        char* response = NULL;
        size_t responseSize = 0;

        // Set URL and other options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Response:\n%s\n", response);
        }

        curl_easy_cleanup(curl);
        free(response);
    }

    return 0;
}
