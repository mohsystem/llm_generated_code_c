#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT 2048

// دالة لتشغيل برنامج CGI مع ضبط REQUEST_METHOD وتمرير بيانات POST عبر echo
int run_cgi_with_method_and_data(const char* method, const char* post_data, char* output, size_t max_output) {
    setenv("REQUEST_METHOD", method, 1);

    FILE* fp = NULL;
    char cmd[1024];

    if (post_data != NULL) {
        snprintf(cmd, sizeof(cmd), "echo \"%s\" | ./cgi_program", post_data);
        fp = popen(cmd, "r");
    } else {
        fp = popen("./cgi_program", "r");
    }

    if (!fp) {
        snprintf(output, max_output, "Failed to run CGI program\n");
        return 1;
    }

    output[0] = '\0';
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strlen(output) + strlen(buffer) < max_output) {
            strcat(output, buffer);
        } else {
            break;
        }
    }

    int status = pclose(fp);
    if (status != 0) return 1;
    return 0;
}

void run_tests() {
    struct {
        const char* method;
        const char* post_data;
        const char* expected_fragment;
        const char* description;
    } tests[] = {
        {"GET", NULL, "<form method='POST'>", "Test 1: GET returns form"},
        {"POST", "csrf_token=your_csrf_token_here&username=alice&email=a@example.com", "Settings updated successfully!", "Test 2: POST with valid CSRF"},
        {"POST", "username=alice&email=a@example.com", "CSRF token validation failed", "Test 3: POST missing CSRF"},
        {"POST", "csrf_token=wrong_token&username=alice&email=a@example.com", "CSRF token validation failed", "Test 4: POST invalid CSRF"},
    };

    char output[MAX_OUTPUT];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i=0; i<n; i++) {
        int ret = run_cgi_with_method_and_data(tests[i].method, tests[i].post_data, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if (ret == 0 && strstr(output, tests[i].expected_fragment) != NULL) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected fragment: %s\nGot output:\n%s\n", tests[i].expected_fragment, output);
        }
    }
}

int main() {
    run_tests();
    return 0;
}
