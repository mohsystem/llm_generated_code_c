#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    // Allocate memory
    int* ptr = (int*)malloc(sizeof(int));
    assert(ptr != NULL); // Test case 1: Allocation successful

    // Use the allocated memory
    *ptr = 10;
    assert(*ptr == 10); // Test case 2: Memory usage correctness

    // Deallocate memory
    free(ptr);

    // After free, ptr is dangling; no test needed, but pointer set to NULL for safety
    ptr = NULL;
    assert(ptr == NULL); // Test case 3: Pointer set to NULL after free (good practice)

    printf("All test cases passed!\n");
    return 0;
}
