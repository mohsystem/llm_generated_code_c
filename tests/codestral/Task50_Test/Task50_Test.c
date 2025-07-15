#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>  // سنستخدم libcurl لإرسال POST مع ملف

// URL السيرفر
#define SERVER_URL "http://127.0.0.1:8080/upload"
// مسار الملف المرفوع
#define FILE_PATH "/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task50_Test/testFile.txt"

int main() {
    CURL *curl;
    CURLcode res;
    int test_passed = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;

        // إضافة الملف إلى الفورم
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_FILE, FILE_PATH,
                     CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        // لتعطيل الطباعة التلقائية
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        // تنفيذ الطلب
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "Failed to upload file: %s\n", curl_easy_strerror(res));
            printf("Test case: FAIL\n");
        } else {
            long response_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            if(response_code == 200) {
                printf("Test case: PASS\n");
                test_passed = 1;
            } else {
                printf("Test case: FAIL (HTTP Status: %ld)\n", response_code);
            }
        }

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    } else {
        printf("Failed to init curl\nTest case: FAIL\n");
    }
    curl_global_cleanup();

    return test_passed ? 0 : 1;
}
