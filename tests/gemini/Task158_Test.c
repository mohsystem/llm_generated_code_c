#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int runTest(const char* input) {
    char command[1024];
    strncpy(command, input, sizeof(command)-1);
    command[sizeof(command)-1] = '\0';

    // نحذف \n لو موجودة لان fgets تضيفها
    size_t len = strlen(command);
    if (len > 0 && command[len-1] == '\n') {
        command[len-1] = '\0';
    }

    int ret = system(command);
    return ret == 0;
}

int main() {
    // Test 1: simple echo command
    assert(runTest("echo Hello\n"));

    // Test 2: echo with spaces
    assert(runTest("echo Hello World\n"));

    // Test 3: empty command (just newline)
    assert(runTest("\n"));

    // Test 4: command with special chars
    assert(runTest("echo !@#$%^&*()\n"));

    // Test 5: long command
    char longCommand[1024];
    for (int i = 0; i < 1022; i++) longCommand[i] = 'a';
    longCommand[1022] = '\n';
    longCommand[1023] = '\0';
    assert(runTest(longCommand));

    printf("All test cases passed!\n");
    return 0;
}
