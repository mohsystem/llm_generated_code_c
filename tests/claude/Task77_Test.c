#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// دالة كتابة البيانات المستقبلة من الـ FTP إلى ملف محلي
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void program_main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <hostname> <username> <password> <filename>\n", argv[0]);
        return;
    }

    char *hostname = argv[1];
    char *username = argv[2];
    char *password = argv[3];
    char *filename = argv[4];

    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename, "wb");
        if (!fp) {
            printf("Failed to open file %s for writing.\n", filename);
            curl_easy_cleanup(curl);
            return;
        }
        char url[512];
        snprintf(url, sizeof(url), "ftp://%s/%s", hostname, filename);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERNAME, username);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);

        fclose(fp);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            printf("File %s downloaded successfully.\n", filename);
        } else {
            printf("Failed to download the file: %s\n", curl_easy_strerror(res));
        }
    }
}

// دوال مساعدة لالتقاط stdout و stderr
#include <unistd.h>

int capture_output(void (*func)(int,char**), int argc, char* argv[],
                   char* out_buf, size_t out_size,
                   char* err_buf, size_t err_size) {
    int out_pipe[2], err_pipe[2];
    if (pipe(out_pipe) == -1) return -1;
    if (pipe(err_pipe) == -1) return -1;

    fflush(stdout);
    fflush(stderr);

    int stdout_copy = dup(fileno(stdout));
    int stderr_copy = dup(fileno(stderr));
    if (stdout_copy == -1 || stderr_copy == -1) return -1;

    dup2(out_pipe[1], fileno(stdout));
    dup2(err_pipe[1], fileno(stderr));
    close(out_pipe[1]);
    close(err_pipe[1]);

    func(argc, argv);

    fflush(stdout);
    fflush(stderr);

    dup2(stdout_copy, fileno(stdout));
    dup2(stderr_copy, fileno(stderr));
    close(stdout_copy);
    close(stderr_copy);

    ssize_t out_len = read(out_pipe[0], out_buf, out_size - 1);
    ssize_t err_len = read(err_pipe[0], err_buf, err_size - 1);
    close(out_pipe[0]);
    close(err_pipe[0]);

    if (out_len < 0 || err_len < 0) return -1;

    out_buf[out_len] = '\0';
    err_buf[err_len] = '\0';

    return 0;
}

void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

// حالات الاختبار الصارمة كما في البايثون

void test_wrong_argc() {
    char out[1024] = {0}, err[1024] = {0};
    char* argv[] = {"program"};
    int argc = 1;
    int ret = capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err));
    print_result("test_wrong_argc", ret == 0 && strstr(out, "Usage:") != NULL);
}

// نلاحظ أن اختبار تنزيل ملف FTP ناجح يحتاج إلى سيرفر FTP يعمل ومعلومات صحيحة، لذلك نطبع "Not implemented" لهذا الاختبار
void test_successful_download() {
    print_result("test_successful_download", 0);
    printf("test_successful_download: FAIL (not implemented)\n");
}

// اختبار فشل التنزيل بسبب بيانات ناقصة أو خاطئة (مثلاً اسم مستخدم أو كلمة مرور خاطئة)
// هنا نختبر أن البرنامج يعيد خطأ (لكنه يعتمد على السيرفر الخارجي لذلك ممكن يصعب التحقق بدقة)
void test_failed_download() {
    print_result("test_failed_download", 0);
    printf("test_failed_download: FAIL (not implemented)\n");
}

int main() {
    test_wrong_argc();
    test_successful_download();
    test_failed_download();
    return 0;
}
