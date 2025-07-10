#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1024

// دالة لتشغيل البرنامج CGI محاكاة مع ضبط QUERY_STRING وقراءة الناتج
int run_cgi_with_input(const char* user_input, char* output, size_t max_output_size) {
    // ضبط متغير البيئة
    char env_var[256];
    snprintf(env_var, sizeof(env_var), "user_input=%s", user_input);
    setenv("QUERY_STRING", env_var, 1);

    // التقاط الإخراج عن طريق إعادة توجيه stdout إلى ملف مؤقت
    FILE* fp = popen("./cgi_program", "r"); // استبدل ./cgi_program باسم البرنامج إذا مختلف
    if (fp == NULL) {
        snprintf(output, max_output_size, "Failed to run CGI program");
        return 1;
    }

    output[0] = '\0';
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(output) + strlen(buffer) < max_output_size) {
            strcat(output, buffer);
        }
    }

    pclose(fp);
    return 0;
}

void run_tests() {
    struct {
        const char* input;
        const char* expected_fragment;  // جزء من الناتج المتوقّع
        const char* description;
    } tests[] = {
        {"HelloWorld", "<h1>Your input: HelloWorld</h1>", "Test Case 1: Normal input"},
        {"", "<h1>Your input: </h1>", "Test Case 2: Empty input"},
        {"<script>alert(1)</script>", "<h1>Your input: <script>alert(1)</script></h1>", "Test Case 3: HTML tags"},
        {"name=John+Doe", "<h1>Your input: name=John+Doe</h1>", "Test Case 4: Query like input"},
    };

    char output[MAX_OUTPUT_SIZE];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < n; i++) {
        int ret = run_cgi_with_input(tests[i].input, output, sizeof(output));
        printf("Running %s... ", tests[i].description);
        if (ret == 0 && strstr(output, tests[i].expected_fragment) != NULL) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected to find: %s\nGot output:\n%s\n", tests[i].expected_fragment, output);
        }
    }
}

int main() {
    run_tests();
    return 0;
}

