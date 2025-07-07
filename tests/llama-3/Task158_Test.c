#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int runTest(const char* input, const char* expectedOutput) {
    char data[100];
    strncpy(data, input, sizeof(data) - 1);
    data[sizeof(data) - 1] = '\0';

    // إزالة ال newline إذا موجودة
    size_t len = strlen(data);
    if (len > 0 && data[len - 1] == '\n') {
        data[len - 1] = '\0';
    }

    char command[150];
    snprintf(command, sizeof(command), "echo %s > output.txt", data);

    int ret = system(command);
    if (ret != 0) {
        return 0; // فشل تنفيذ الأمر
    }

    // اقرأ محتوى output.txt وتحقق من تطابقه مع expectedOutput
    FILE* f = fopen("output.txt", "r");
    if (!f) return 0;

    char buffer[200];
    if (!fgets(buffer, sizeof(buffer), f)) {
        fclose(f);
        return 0;
    }
    fclose(f);

    // إزالة newline من الملف إذا موجودة
    size_t blen = strlen(buffer);
    if (blen > 0 && buffer[blen - 1] == '\n') {
        buffer[blen - 1] = '\0';
    }

    return strcmp(buffer, expectedOutput) == 0;
}

int main() {
    // Test 1
    assert(runTest("Hello", "Hello"));

    // Test 2
    assert(runTest("Hello World", "Hello World"));

    // Test 3 empty string input
    assert(runTest("", ""));

    // Test 4 special characters
    assert(runTest("!@#$%^&*()", "!@#$%^&*()"));

    // Test 5 long input (100 'a's)
    char longInput[101];
    for (int i = 0; i < 100; i++) longInput[i] = 'a';
    longInput[100] = '\0';
    assert(runTest(longInput, longInput));

    printf("All test cases passed!\n");
    return 0;
}
