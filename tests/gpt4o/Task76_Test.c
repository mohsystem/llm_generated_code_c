#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h> // pipe, dup2, read, close

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void program_main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        return;
    }

    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        char *title_start = strstr(chunk.memory, "<title>");
        char *title_end = strstr(chunk.memory, "</title>");
        if (title_start && title_end) {
            title_start += 7; // Move pointer past '<title>'
            *title_end = '\0'; // Null-terminate the string
            printf("Page title: %s\n", title_start);
        } else {
            fprintf(stderr, "Error: Could not extract title from HTML\n");
        }
    }

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();
}

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

void test_wrong_argc() {
    char out[1024] = {0}, err[1024] = {0};
    char* argv[] = {"program"};
    int argc = 1;
    int ret = capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err));
    print_result("test_wrong_argc", ret == 0 && (strstr(err, "Usage:") != NULL));
}

void test_valid_url() {
    char out[8192] = {0}, err[1024] = {0};
    char* argv[] = {"program", "https://www.google.com/"};
    int argc = 2;
    int ret = capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err));
    print_result("test_valid_url", ret == 0 && (strstr(out, "Page title:") != NULL));
}

void test_invalid_url() {
    char out[8192] = {0}, err[1024] = {0};
    char* argv[] = {"program", "http://invalid.url.fake"};
    int argc = 2;
    int ret = capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err));
    int pass = (strstr(err, "curl_easy_perform() failed") != NULL) || (strstr(err, "Could not extract title") != NULL);
    print_result("test_invalid_url", ret == 0 && pass);
}

int main() {
    test_wrong_argc();
    test_valid_url();
    test_invalid_url();
    return 0;
}
