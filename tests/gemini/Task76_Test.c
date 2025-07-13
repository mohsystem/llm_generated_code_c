#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <regex.h>
#include <unistd.h> // pipe, dup2, read, close

// Callback function for cURL to write data to a string
size_t write_callback(void *contents, size_t size, size_t nmemb, char **buffer) {
    size_t newLength = size * nmemb;
    *buffer = (char *)realloc(*buffer, strlen(*buffer) + newLength + 1);
    if (*buffer == NULL) {
        fprintf(stderr, "Error: realloc failed\n");
        return 0;
    }
    memcpy(*buffer + strlen(*buffer), contents, newLength);
    (*buffer)[strlen(*buffer) + newLength] = '\0';
    return newLength;
}

// Function to extract title from HTML
char *extract_title(const char *html) {
    regex_t regex;
    regmatch_t match[2];
    char *title = NULL;
    int reti;

    // Compile regular expression
    reti = regcomp(&regex, "<title>(.*?)</title>", REG_ICASE | REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Error: Could not compile regex\n");
        return NULL;
    }

    // Execute regular expression
    reti = regexec(&regex, html, 2, match, 0);
    if (!reti) {
        // Extract matched substring
        int start = match[1].rm_so;
        int end = match[1].rm_eo;
        int title_len = end - start;
        title = (char *)malloc(title_len + 1);
        if (title == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            regfree(&regex);
            return NULL;
        }
        strncpy(title, html + start, title_len);
        title[title_len] = '\0';
    } else if (reti != REG_NOMATCH) {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Error: Regex match failed: %s\n", msgbuf);
    }

    regfree(&regex);
    return title;
}

void program_main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return;
    }

    const char *url = argv[1];
    CURL *curl;
    CURLcode res;
    char *buffer = malloc(1);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate initial buffer\n");
        return;
    }
    buffer[0] = '\0';
    char *title = NULL;

    // Initialize cURL
    curl = curl_easy_init();
    if (curl) {
        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set write callback function and data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        // Perform HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "Error: curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Extract title from HTML
            title = extract_title(buffer);
            if (title != NULL) {
                printf("Page title: %s\n", title);
                free(title);
            } else {
                fprintf(stderr, "Error: Could not extract title from HTML\n");
            }
        }

        // Clean up cURL
        curl_easy_cleanup(curl);

        // Free buffer
        free(buffer);
    } else {
        fprintf(stderr, "Error: Could not initialize cURL\n");
        free(buffer);
    }
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

// Test cases strictly matching the Python code

void test_wrong_argc() {
    char out[1024] = {0}, err[1024] = {0};
    char* argv[] = {"program"};
    int argc = 1;
    int ret = capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err));
    print_result("test_wrong_argc", ret == 0 && (strstr(err, "Usage:") != NULL));
}

void test_valid_url() {
    // You can replace this URL with a known reachable page with title <title>Example Domain</title>
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
    // Because invalid URL prints error to stderr, accept either output with error or output with page title missing
    int pass = (strstr(err, "Error: curl_easy_perform() failed:") != NULL) || (strstr(out, "Error: Could not extract title") != NULL);
    print_result("test_invalid_url", ret == 0 && pass);
}

int main() {
    test_wrong_argc();
    test_valid_url();
    test_invalid_url();
    return 0;
}
