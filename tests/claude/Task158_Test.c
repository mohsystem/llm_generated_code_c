#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_INPUT 1000

// A helper function to capture system command output for testing purposes
// (Note: In real scenarios, system command output capturing is platform dependent
// and may require popen(), which is not always allowed. Here, we keep it simple.)

int main() {
    char userInput[MAX_INPUT];
    char command[MAX_INPUT + 5];  // Extra space for "echo " and null terminator

    // Test Case 1: Simple input
    strcpy(userInput, "HelloWorld");
    snprintf(command, sizeof(command), "echo %s", userInput);
    int ret = system(command);
    assert(ret == 0);

    // Test Case 2: Input with spaces
    strcpy(userInput, "Hello World");
    snprintf(command, sizeof(command), "echo %s", userInput);
    ret = system(command);
    assert(ret == 0);

    // Test Case 3: Empty input
    strcpy(userInput, "");
    snprintf(command, sizeof(command), "echo %s", userInput);
    ret = system(command);
    assert(ret == 0);

    // Test Case 4: Input with special characters
    strcpy(userInput, "special!@#$$%^&*()");
    snprintf(command, sizeof(command), "echo %s", userInput);
    ret = system(command);
    assert(ret == 0);

    // Test Case 5: Long input
    memset(userInput, 'a', MAX_INPUT - 1);
    userInput[MAX_INPUT - 1] = '\0';
    snprintf(command, sizeof(command), "echo %s", userInput);
    ret = system(command);
    assert(ret == 0);

    printf("All test cases passed!\n");

    return 0;
}
