#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int runTest(int *buffer, int bufferSize, int index, int expectedValue) {
    if (index == -1) {
        printf("Test with index -1: Exit\n");
        return 1;  // Passed if handled correctly
    }
    if (index < 0 || index >= bufferSize) {
        printf("Test with index %d: Invalid index.\n", index);
        return 1;  // Passed if handled correctly
    }

    if (buffer[index] == expectedValue) {
        printf("Test with index %d: Buffer[%d] = %d (PASSED)\n", index, index, buffer[index]);
        return 1;
    } else {
        printf("Test with index %d: Expected %d, but found %d. (FAILED)\n", index, expectedValue, buffer[index]);
        return 0;
    }
}

int main() {
    int bufferSize;
    printf("Enter the buffer size: ");
    scanf("%d", &bufferSize);
    int* buffer = (int*) malloc(bufferSize * sizeof(int));

    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Initialize the buffer with values
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = i * 10;  // Example initialization (multiplying index by 10)
    }

    // Define test cases
    struct {
        int index;
        int expectedValue;
    } testCases[] = {
        {0, 0},      // Correct index, expect 0
        {1, 10},     // Correct index, expect 10
        {2, 20},     // Correct index, expect 20
        {bufferSize - 1, (bufferSize - 1) * 10}, // Last index, expect correct value
        {bufferSize, -1},  // Out of bounds index
        {-1, -1},    // Negative index
    };

    int totalTests = sizeof(testCases) / sizeof(testCases[0]);
    int passed = 0;

    // Run the tests
    for (int i = 0; i < totalTests; i++) {
        int idx = testCases[i].index;
        int expected = testCases[i].expectedValue;
        passed += runTest(buffer, bufferSize, idx, expected);
    }

    printf("\nPassed %d/%d tests.\n", passed, totalTests);

    // Free dynamically allocated memory
    free(buffer);
    return 0;
}
