#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_INPUT 100
#define MAX_COMMAND 200

int runTest(const char* input) {
    char command[MAX_COMMAND];
    // تأكد من إزالة \n من input لو موجودة
    char safeInput[MAX_INPUT];
    strncpy(safeInput, input, MAX_INPUT - 1);
    safeInput[MAX_INPUT - 1] = '\0';
    size_t len = strlen(safeInput);
    if(len > 0 && safeInput[len - 1] == '\n') {
        safeInput[len - 1] = '\0';
    }

    snprintf(command, sizeof(command), "echo %s", safeInput);
    int ret = system(command);
    return ret == 0;
}

int main() {
    // Test Case 1: Simple input
    assert(runTest("HelloWorld\n"));

    // Test Case 2: Input with spaces
    assert(runTest("Hello World\n"));

    // Test Case 3: Empty input (just newline)
    assert(runTest("\n"));

    // Test Case 4: Special characters
    assert(runTest("!@#$%^&*()_+\n"));

    // Test Case 5: Long input
    char longInput[MAX_INPUT];
    for (int i = 0; i < MAX_INPUT - 2; i++) {
        longInput[i] = 'a';
    }
    longInput[MAX_INPUT - 2] = '\n';
    longInput[MAX_INPUT - 1] = '\0';
    assert(runTest(longInput));

    printf("All test cases passed!\n");
    return 0;
}
