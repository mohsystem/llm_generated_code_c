#include <stdio.h>
#include <string.h>

#define MAX_SIZE 40001

typedef struct {
    int products[MAX_SIZE];
    int size;
} ProductOfNumbers;

void ProductOfNumbersInit(ProductOfNumbers* obj) {
    obj->products[0] = 1;
    obj->size = 1;
}

void ProductOfNumbersAdd(ProductOfNumbers* obj, int num) {
    if (num == 0) {
        obj->products[0] = 1;
        obj->size = 1;
    } else {
        obj->products[obj->size] = obj->products[obj->size - 1] * num;
        obj->size++;
    }
}

int ProductOfNumbersGetProduct(ProductOfNumbers* obj, int k) {
    if (k >= obj->size) {
        return 0;
    }
    return obj->products[obj->size - 1] / obj->products[obj->size - k - 1];
}

void runTest(const char* name, int expected, int actual) {
    if (expected == actual) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", name, expected, actual);
    }
}

int main() {
    ProductOfNumbers productOfNumbers;
    ProductOfNumbersInit(&productOfNumbers);

    ProductOfNumbersAdd(&productOfNumbers, 3);
    ProductOfNumbersAdd(&productOfNumbers, 0);
    ProductOfNumbersAdd(&productOfNumbers, 2);
    ProductOfNumbersAdd(&productOfNumbers, 5);
    ProductOfNumbersAdd(&productOfNumbers, 4);

    runTest("Test Case 1", 20, ProductOfNumbersGetProduct(&productOfNumbers, 2));  // 5*4=20
    runTest("Test Case 2", 40, ProductOfNumbersGetProduct(&productOfNumbers, 3));  // 2*5*4=40
    runTest("Test Case 3", 0, ProductOfNumbersGetProduct(&productOfNumbers, 4));   // 0*2*5*4=0

    ProductOfNumbersAdd(&productOfNumbers, 8);
    runTest("Test Case 4", 32, ProductOfNumbersGetProduct(&productOfNumbers, 2));  // 4*8=32

    ProductOfNumbersAdd(&productOfNumbers, 6);
    runTest("Test Case 5", 192, ProductOfNumbersGetProduct(&productOfNumbers, 3)); // 8*6*4=192

    // Reset for next tests
    ProductOfNumbersInit(&productOfNumbers);

    ProductOfNumbersAdd(&productOfNumbers, 5);
    runTest("Test Case 6", 5, ProductOfNumbersGetProduct(&productOfNumbers, 1));   // 5

    ProductOfNumbersAdd(&productOfNumbers, 0);
    runTest("Test Case 7", 0, ProductOfNumbersGetProduct(&productOfNumbers, 1));   // 0

    ProductOfNumbersAdd(&productOfNumbers, 1);
    ProductOfNumbersAdd(&productOfNumbers, 2);
    ProductOfNumbersAdd(&productOfNumbers, 3);
    ProductOfNumbersAdd(&productOfNumbers, 4);
    runTest("Test Case 8", 24, ProductOfNumbersGetProduct(&productOfNumbers, 4));  // 1*2*3*4=24

    // Reset for next tests
    ProductOfNumbersInit(&productOfNumbers);

    ProductOfNumbersAdd(&productOfNumbers, 1);
    ProductOfNumbersAdd(&productOfNumbers, 2);
    ProductOfNumbersAdd(&productOfNumbers, 3);
    ProductOfNumbersAdd(&productOfNumbers, 4);
    ProductOfNumbersAdd(&productOfNumbers, 5);
    runTest("Test Case 9", 60, ProductOfNumbersGetProduct(&productOfNumbers, 3));  // 3*4*5=60

    ProductOfNumbersAdd(&productOfNumbers, 0);
    runTest("Test Case 10", 0, ProductOfNumbersGetProduct(&productOfNumbers, 2));  // 0

    return 0;
}
