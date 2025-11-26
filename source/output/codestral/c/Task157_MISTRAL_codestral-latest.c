#include <stdlib.h>

int main() {
    int* myArray;

    // Dynamically allocate memory
    myArray = malloc(10 * sizeof(int));

    // Deallocate memory
    free(myArray);

    return 0;
}
