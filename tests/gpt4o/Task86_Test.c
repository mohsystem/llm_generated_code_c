#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

// دالة كتابة البيانات إلى ملف
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
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
    char *url = "https://download.sysinternals.com/files/ProcessExplorer.exe";  // استبدل بالرابط الصحيح
    char *filename = "downloaded_program.exe";
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename, "wb");
        if (!fp) {
            fprintf(stderr, "Error opening file for writing\n");
            curl_easy_cleanup(curl);
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            printf("Test case 1: fail (download failed)\n");
            return 1;
        }

        if (check_file_exists_and_not_empty(filename)) {
            printf("Test case 1: pass (file downloaded successfully)\n");
        } else {
            printf("Test case 1: fail (file missing or empty)\n");
            return 1;
        }

        // تشغيل الملف
        system(filename);
    } else {
        printf("Test case 1: fail (curl init failed)\n");
        return 1;
    }

    return 0;
}
