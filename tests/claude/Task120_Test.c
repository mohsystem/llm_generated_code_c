#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        // out of memory!
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void print_result(const char* test_name, long response_code, int success) {
    printf("%s: ", test_name);
    if (success && response_code == 200) {
        printf("PASS\n");
    } else {
        printf("FAIL (HTTP status code: %ld)\n", response_code);
    }
}

void test_get_homepage(const char* url, const char* test_name) {
    CURL *curl;
    CURLcode res;
    long response_code = 0;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            printf("%s: FAIL (curl_easy_perform() failed: %s)\n", test_name, curl_easy_strerror(res));
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            // Print result and some content preview (first 100 chars)
            print_result(test_name, response_code, 1);
            // Uncomment below if you want to print content (optional)
            // printf("Response content (truncated): %.100s\n", chunk.memory);
        }

        curl_easy_cleanup(curl);
    } else {
        printf("%s: FAIL (curl init failed)\n", test_name);
    }

    free(chunk.memory);
}

void test_post_user_input(const char* url, const char* user_input, const char* test_name) {
    CURL *curl;
    CURLcode res;
    long response_code = 0;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk;
    char postfields[1100];  // enough for user_input 1000 + extra

    chunk.memory = malloc(1);
    chunk.size = 0;

    snprintf(postfields, sizeof(postfields), "user_input=%s", user_input);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postfields));

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Set content-type header for form submission
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            printf("%s: FAIL (curl_easy_perform() failed: %s)\n", test_name, curl_easy_strerror(res));
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            print_result(test_name, response_code, 1);
            // Uncomment below if you want to print content (optional)
            // printf("Response content (truncated): %.100s\n", chunk.memory);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        printf("%s: FAIL (curl init failed)\n", test_name);
    }

    free(chunk.memory);
}

int main(void) {
    const char* base_url = "http://127.0.0.1:5000/";

    // Test Case 1: GET request to the homepage
    test_get_homepage(base_url, "Test Case 1: GET request to the homepage");

    // Test Case 2: POST request with input 'Hello, World!'
    test_post_user_input(base_url, "Hello,%20World!", "Test Case 2: POST request with input 'Hello, World!'");

    // Test Case 3: POST request with empty input
    test_post_user_input(base_url, "", "Test Case 3: POST request with empty input");

    // Test Case 4: POST request with input containing special characters
    test_post_user_input(base_url, "!@#$%^&*()_+", "Test Case 4: POST request with input containing special characters");

    // Test Case 5: POST request with a long string (1000 'A's)
    char long_string[1001];
    memset(long_string, 'A', 1000);
    long_string[1000] = '\0';
    test_post_user_input(base_url, long_string, "Test Case 5: POST request with a long string");

    // Test Case 6: GET request to the homepage after previous POST requests
    test_get_homepage(base_url, "Test Case 6: GET request to the homepage after previous POST requests");

    // Test Case 7: POST request with input containing HTML tags
    test_post_user_input(base_url, "<script>alert(%22Test%22)</script>", "Test Case 7: POST request with input containing HTML tags");

    // Test Case 8: POST request with numeric input
    test_post_user_input(base_url, "123456", "Test Case 8: POST request with numeric input");

    // Test Case 9: POST request with whitespace input
    test_post_user_input(base_url, "   ", "Test Case 9: POST request with whitespace input");

    // Test Case 10: POST request with a mix of alphanumeric and special characters
    test_post_user_input(base_url, "Test123!@#", "Test Case 10: POST request with a mix of alphanumeric and special characters");

    return 0;
}
