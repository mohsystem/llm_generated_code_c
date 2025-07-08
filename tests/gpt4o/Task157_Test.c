#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    // Allocate memory for an integer
    int *int_ptr = (int *)malloc(sizeof(int));
    assert(int_ptr != NULL);  // Test case 1: Allocation successful

    *int_ptr = 10;
    assert(*int_ptr == 10);   // Test case 2: Memory usage correctness

    printf("Value at allocated memory: %d\n", *int_ptr);

    // Deallocate memory
    free(int_ptr);

    int_ptr = NULL;           // Good practice to avoid dangling pointer
    assert(int_ptr == NULL);  // Test case 3: Pointer set to NULL after free

    printf("All test cases passed!\n");

    return 0;
}
