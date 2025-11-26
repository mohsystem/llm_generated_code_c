#include <stdio.h>
#include <string.h>

// الدالة الأصلية (تطبع)
void processInput(const char* input) {
    printf("Processed input: %s\n", input);
}

// نسخة للاختبار ترجع الناتج في buffer بدل الطباعة
void processInputToString(const char* input, char* buffer, size_t max_len) {
    snprintf(buffer, max_len, "Processed input: %s\n", input);
}

// دالة اختبار داخلية بدون أي ملفات
void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        const char* name;
    } tests[] = {
        {"Hello, world!", "Processed input: Hello, world!\n", "Test 1"},
        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "Processed input: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n", "Test 2"},
        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "Processed input: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n", "Test 3"},
        {"", "Processed input: \n", "Test 4"},
        {"@#$%^&*()", "Processed input: @#$%^&*()\n", "Test 5"},
        {"1234567890", "Processed input: 1234567890\n", "Test 6"},
        {"Hello123", "Processed input: Hello123\n", "Test 7"},
        {"     ", "Processed input:      \n", "Test 8"},
        {"Hello\nWorld", "Processed input: Hello\nWorld\n", "Test 9"},
        {"  Hello  ", "Processed input:   Hello  \n", "Test 10"},
    };

    char buffer[512];
    int n = sizeof(tests)/sizeof(tests[0]);
    for(int i=0; i<n; i++) {
        processInputToString(tests[i].input, buffer, sizeof(buffer));
        if(strcmp(buffer, tests[i].expected_output) == 0) {
            printf("%s: PASS\n", tests[i].name);
        } else {
            printf("%s: FAIL\nExpected:\n%sGot:\n%s\n", tests[i].name, tests[i].expected_output, buffer);
        }
    }
}

int main() {
    run_tests();
    return 0;
}
