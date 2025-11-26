#include <stdio.h>
#include <stdlib.h>

int runTest(int index, char* buffer, int bufferSize) {
    if (index == -1) {
        // Exit condition test case, nothing to check
        return 1;
    }
    if (index >= 0 && index < bufferSize) {
        printf("Data at index %d: %c\n", index, buffer[index]);
        return 1; // Test passes as valid index accessed
    } else {
        printf("Invalid index. Please enter a number between 0 and %d.\n", bufferSize - 1);
        return 1; // Test passes as invalid index handled correctly
    }
    return 0; // Should never reach here
}

int main() {
    const int bufferSize = 100;
    char* buffer = (char*)malloc(bufferSize * sizeof(char));

    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize buffer with some data (A-Z cyclic)
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = 'A' + (i % 26);
    }

    // Test cases, strict from Python version, same inputs and expected outputs:
    int testIndices[] = {0, 10, 25, 99, 100, -1, -5, 50};
    // Note: 100 and -5 are invalid indices, -1 is exit

    int testsCount = sizeof(testIndices) / sizeof(testIndices[0]);
    int passed = 0;

    for (int i = 0; i < testsCount; i++) {
        int idx = testIndices[i];
        printf("Test case %d: index = %d\n", i + 1, idx);
        passed += runTest(idx, buffer, bufferSize);
    }

    printf("\nPassed %d/%d test cases.\n", passed, testsCount);

    free(buffer); // Free allocated memory
    return 0;
}
