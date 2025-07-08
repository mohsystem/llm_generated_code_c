#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    int* ptr = (int*)malloc(sizeof(int));
    assert(ptr != NULL);               // Test 1: malloc success first time
    *ptr = 10;
    assert(*ptr == 10);                // Test 2: value correctly assigned first time
    printf("%d\n", *ptr);

    free(ptr);

    ptr = (int*)malloc(sizeof(int));
    assert(ptr != NULL);               // Test 3: malloc success second time
    *ptr = 20;
    assert(*ptr == 20);                // Test 4: value correctly assigned second time
    printf("%d\n", *ptr);

    free(ptr);
    ptr = NULL;
    assert(ptr == NULL);               // Test 5: pointer set to NULL after free

    printf("All test cases passed!\n");
    return 0;
}
