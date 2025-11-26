#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>

// دالة كتابة البيانات إلى الملف
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    fwrite(ptr, size, nmemb, (FILE*)stream);
    return size * nmemb;
}

// تحقق من وجود الملف وحجمه > 0
int check_file_exists_and_not_empty(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

int main() {
    char url[256];
    printf("Enter the URL of the EXE file: ");
    scanf("%255s", url);

    char *filename = strrchr(url, '/');
    filename = filename ? filename + 1 : url;

    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file for writing\n");
        printf("Test case 1: fail (file open error)\n");
        return 1;
    }

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(file);

        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
            printf("Test case 1: fail (download failed)\n");
            curl_global_cleanup();
            return 1;
        }

        if (check_file_exists_and_not_empty(filename)) {
            printf("Test case 1: pass (file downloaded successfully)\n");
        } else {
            printf("Test case 1: fail (file missing or empty)\n");
            curl_global_cleanup();
            return 1;
        }

        // تنفيذ الملف
        system(filename);
    } else {
        printf("Test case 1: fail (curl init failed)\n");
        curl_global_cleanup();
        return 1;
    }

    curl_global_cleanup();
    return 0;
}

// URL: https://download.sysinternals.com/files/ProcessExplorer.exe