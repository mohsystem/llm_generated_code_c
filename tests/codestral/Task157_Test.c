#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    int* myArray;

    // Test 1: Allocate memory and check pointer not NULL
    myArray = malloc(10 * sizeof(int));
    assert(myArray != NULL && "Test Case 1 Failed: malloc returned NULL");

    // Test 2: Write/read a value
    myArray[0] = 123;
    assert(myArray[0] == 123 && "Test Case 2 Failed: memory read/write mismatch");

    // Test 3: Free the allocated memory (cannot test free but ensure no crash)
    free(myArray);

    // Test 4: Allocate zero bytes (platform-dependent behavior)
    int* zeroAlloc = malloc(0);
    // Just free if not NULL, no assert
    if (zeroAlloc != NULL) {
        free(zeroAlloc);
    }

    printf("All test cases passed!\n");
    return 0;
}
