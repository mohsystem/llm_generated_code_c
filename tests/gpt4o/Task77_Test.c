#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // pipe, dup2, read, close

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
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
    char ftp_url[256];

    snprintf(ftp_url, sizeof(ftp_url), "ftp://%s/%s", hostname, filename);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename, "wb");
        if (!fp) {
            printf("Failed to open file %s for writing.\n", filename);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        }
        // WARNING: The following usage of strcat is unsafe; however, per your instructions, do not modify original code.
        // If you want safety, this must be fixed.
        char userpwd[512];
        snprintf(userpwd, sizeof(userpwd), "%s:%s", username, password);
        curl_easy_setopt(curl, CURLOPT_URL, ftp_url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        if (res == CURLE_OK) {
            printf("File %s downloaded successfully.\n", filename);
        } else {
            printf("Failed to download the file: %s\n", curl_easy_strerror(res));
        }
    } else {
        printf("Failed to initialize curl.\n");
    }
    curl_global_cleanup();
}

// Helper functions for capturing stdout and stderr
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

// Strict test cases exactly from the Python code

void test_wrong_argc() {
    char out[1024] = {0}, err[1024] = {0};
    char* argv[] = {"program"};
    int argc = 1;
    int ret = capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err));
    print_result("test_wrong_argc", ret == 0 && strstr(out, "Usage:") != NULL);
}

// These two tests require actual FTP connection and credentials and are environment-dependent,
// so they are marked as not implemented per instructions.
void test_successful_download() {
    print_result("test_successful_download", 0);
    printf("test_successful_download: FAIL (not implemented)\n");
}

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
