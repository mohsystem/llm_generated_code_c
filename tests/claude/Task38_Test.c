#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int test_post_request(const char* url, const char* post_fields, const char* expected_substring) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(!curl) {
        printf("Failed to init curl\n");
        free(chunk.memory);
        return 0;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);

    int pass = 0;
    if(res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        if (strstr(chunk.memory, expected_substring) != NULL) {
            pass = 1;
        }
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    free(chunk.memory);

    return pass;
}

int main() {
    // افترض ان السيرفر يعمل على localhost:8080

    struct {
        const char* input_name;
        const char* expected_html_fragment;
        const char* description;
    } tests[] = {
        {"Ahmad", "Hello, Ahmad!", "Test Case 1: Simple name"},
        {"", "Hello, !", "Test Case 2: Empty name"},
        {"<script>", "Hello, <script>!", "Test Case 3: Special chars"},
    };

    int n = sizeof(tests)/sizeof(tests[0]);
    for (int i=0; i<n; i++) {
        char postfields[128];
        snprintf(postfields, sizeof(postfields), "name=%s", tests[i].input_name);
        printf("Running %s... ", tests[i].description);
        int pass = test_post_request("http://localhost:8080/", postfields, tests[i].expected_html_fragment);
        printf(pass ? "PASS\n" : "FAIL\n");
    }

    return 0;
}
