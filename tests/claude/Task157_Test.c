#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    // Test 1: Allocate memory and check if pointer is not NULL
    int* ptr = (int*)malloc(10 * sizeof(int));
    assert(ptr != NULL && "Test Case 1 Failed: Memory allocation returned NULL");

    // Test 2: Use allocated memory and verify value set/read
    ptr[0] = 5;
    assert(ptr[0] == 5 && "Test Case 2 Failed: Memory write/read mismatch");

    // Test 3: Free allocated memory (cannot really test free success, but ensure no crash)
    free(ptr);

    // Test 4: Allocate zero bytes (should return non-NULL or NULL depending on system)
    int* ptr_zero = (int*)malloc(0);
    // This may be NULL or non-NULL, just check no crash:
    // So no assert here, just free if not NULL
    if(ptr_zero) free(ptr_zero);

    // Test 5: Allocate large memory block and check
    int large_size = 1000000;
    int* ptr_large = (int*)malloc(large_size * sizeof(int));
    if(ptr_large == NULL) {
        printf("Warning: Large memory allocation failed (expected on low memory systems).\n");
    } else {
        ptr_large[large_size-1] = 42;
        assert(ptr_large[large_size-1] == 42 && "Test Case 5 Failed: Large memory access mismatch");
        free(ptr_large);
    }

    printf("All test cases passed!\n");
    return 0;
}
