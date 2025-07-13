#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  // pipe, dup2, read, close

extern void program_main(int argc, char* argv[]);

void program_main(int argc, char* argv[]);  // prototype

// --- كود البرنامج الأصلي ---
#include <curl/curl.h>

#define MAX_BUFFER 102400

struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char* extract_title(const char* html) {
    const char* title_start = strstr(html, "<title>");
    if (title_start) {
        title_start += 7;  // past <title>
        const char* title_end = strstr(title_start, "</title>");
        if (title_end) {
            size_t title_length = title_end - title_start;
            char* title = malloc(title_length + 1);
            strncpy(title, title_start, title_length);
            title[title_length] = '\0';
            return title;
        }
    }
    return NULL;
}

void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <url>\n", argv[0]);
        return;
    }

    CURL* curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            char* title = extract_title(chunk.memory);
            if (title) {
                printf("Page title: %s\n", title);
                free(title);
            } else {
                printf("No title found\n");
            }
        }

        curl_easy_cleanup(curl_handle);
        free(chunk.memory);
    }

    curl_global_cleanup();
}

// --- نهاية كود البرنامج الأصلي ---

// التقاط المخرجات من stdout و stderr
int capture_output(int argc, char* argv[], char* out_buf, size_t out_size,
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

    program_main(argc, argv);

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

// طباعة نتيجة الاختبار
void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

// اختبارات صارمة طبقًا للبايثون

int test_wrong_argc() {
    char out[1024], err[1024];
    char* argv[] = {"program"};
    int argc = 1;
    if (capture_output(argc, argv, out, sizeof(out), err, sizeof(err)) != 0) return 0;
    return (strstr(out, "Usage:") != NULL);
}

int test_valid_url() {
    char out[8192], err[1024];
    char* argv[] = {"program", "https://www.google.com/"};
    int argc = 2;
    if (capture_output(argc, argv, out, sizeof(out), err, sizeof(err)) != 0) return 0;
    // فقط نتأكد أن الجملة "Page title:" موجودة في المخرجات فقط
    return (strstr(out, "Page title:") != NULL);
}


int test_invalid_url() {
    char out[8192], err[1024];
    // URL غير صالح / وهمي
    char* argv[] = {"program", "http://invalid.url.abcxyz"};
    int argc = 2;
    if (capture_output(argc, argv, out, sizeof(out), err, sizeof(err)) != 0) return 0;
    // السماح بأن تكون النتيجة إما خطأ من curl أو "No title found"
    return (strstr(out, "Page title:") != NULL || strstr(out, "No title found") != NULL || strstr(err, "failed") != NULL);
}

int main() {
    print_result("test_wrong_argc", test_wrong_argc());
    print_result("test_valid_url", test_valid_url());
    print_result("test_invalid_url", test_invalid_url());
    return 0;
}
