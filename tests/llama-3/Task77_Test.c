#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h> // pipe, dup2, read, close

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    FILE *fp = (FILE *)userp;
    fwrite(contents, size, nmemb, fp);
    return size * nmemb;
}

void program_main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <hostname> <username> <password> <filename>\n", argv[0]);
        return;
    }

    CURL *curl;
    CURLcode res;
    FILE *fp;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        char url[512];
        // Construct ftp url with embedded credentials
        sprintf(url, "ftp://%s:%s@%s/%s", argv[2], argv[3], argv[1], argv[4]);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        fp = fopen(argv[4], "wb");
        if (!fp) {
            printf("Failed to open file %s for writing.\n", argv[4]);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        } else {
            printf("File %s downloaded successfully.\n", argv[4]);
        }
        fclose(fp);
        curl_easy_cleanup(curl);
    } else {
        printf("Failed to initialize curl.\n");
    }
    curl_global_cleanup();
}

// Helper to capture stdout and stderr
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

// Strict test cases exactly from Python

void test_wrong_argc() {
    char out[1024] = {0}, err[1024] = {0};
    char* argv[] = {"program"};
    int argc = 1;
    int ret = capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err));
    print_result("test_wrong_argc", ret == 0 && strstr(out, "Usage:") != NULL);
}

void test_successful_download() {
    // Not implementable without FTP environment, mark fail not implemented
    print_result("test_successful_download", 0);
    printf("test_successful_download: FAIL (not implemented)\n");
}

void test_failed_download() {
    // Not implementable without FTP environment, mark fail not implemented
    print_result("test_failed_download", 0);
    printf("test_failed_download: FAIL (not implemented)\n");
}

int main() {
    test_wrong_argc();
    test_successful_download();
    test_failed_download();
    return 0;
}
