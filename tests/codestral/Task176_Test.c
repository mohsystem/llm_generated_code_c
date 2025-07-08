#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* prefixProducts;
    int size;
    int capacity;
} ProductOfNumbers;

ProductOfNumbers* productOfNumbersCreate() {
    ProductOfNumbers* obj = (ProductOfNumbers*)malloc(sizeof(ProductOfNumbers));
    obj->prefixProducts = (int*)malloc(sizeof(int));
    obj->prefixProducts[0] = 1;
    obj->size = 1;
    obj->capacity = 1;
    return obj;
}

void productOfNumbersAdd(ProductOfNumbers* obj, int num) {
    if (num == 0) {
        obj->prefixProducts[0] = 1;
        obj->size = 1;
    } else {
        if (obj->size == obj->capacity) {
            obj->capacity *= 2;
            obj->prefixProducts = (int*)realloc(obj->prefixProducts, obj->capacity * sizeof(int));
        }
        obj->prefixProducts[obj->size] = obj->prefixProducts[obj->size - 1] * num;
        obj->size++;
    }
}

int productOfNumbersGetProduct(ProductOfNumbers* obj, int k) {
    if (k >= obj->size) {
        return 0;
    } else {
        return obj->prefixProducts[obj->size - 1] / obj->prefixProducts[obj->size - k - 1];
    }
}

void productOfNumbersFree(ProductOfNumbers* obj) {
    free(obj->prefixProducts);
    free(obj);
}

void runTest(const char* name, int expected, int actual) {
    if (expected == actual) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s - Expected: %d, Got: %d\n", name, expected, actual);
    }
}

int main() {
    ProductOfNumbers* productOfNumbers = productOfNumbersCreate();

    productOfNumbersAdd(productOfNumbers, 3);
    productOfNumbersAdd(productOfNumbers, 0);
    productOfNumbersAdd(productOfNumbers, 2);
    productOfNumbersAdd(productOfNumbers, 5);
    productOfNumbersAdd(productOfNumbers, 4);

    runTest("Test Case 1", 20, productOfNumbersGetProduct(productOfNumbers, 2));  // 5*4=20
    runTest("Test Case 2", 40, productOfNumbersGetProduct(productOfNumbers, 3));  // 2*5*4=40
    runTest("Test Case 3", 0, productOfNumbersGetProduct(productOfNumbers, 4));   // 0*2*5*4=0

    productOfNumbersAdd(productOfNumbers, 8);
    runTest("Test Case 4", 32, productOfNumbersGetProduct(productOfNumbers, 2));  // 4*8=32

    productOfNumbersAdd(productOfNumbers, 6);
    runTest("Test Case 5", 192, productOfNumbersGetProduct(productOfNumbers, 3)); // 8*6*4=192

    productOfNumbersFree(productOfNumbers);

    // Test case 6
    productOfNumbers = productOfNumbersCreate();
    productOfNumbersAdd(productOfNumbers, 5);
    runTest("Test Case 6", 5, productOfNumbersGetProduct(productOfNumbers, 1)); // 5

    // Test case 7
    productOfNumbersAdd(productOfNumbers, 0);
    runTest("Test Case 7", 0, productOfNumbersGetProduct(productOfNumbers, 1)); // 0

    // Test case 8
    productOfNumbersAdd(productOfNumbers, 1);
    productOfNumbersAdd(productOfNumbers, 2);
    productOfNumbersAdd(productOfNumbers, 3);
    productOfNumbersAdd(productOfNumbers, 4);
    runTest("Test Case 8", 24, productOfNumbersGetProduct(productOfNumbers, 4)); // 1*2*3*4=24

    // Test case 9
    productOfNumbersFree(productOfNumbers);
    productOfNumbers = productOfNumbersCreate();
    productOfNumbersAdd(productOfNumbers, 1);
    productOfNumbersAdd(productOfNumbers, 2);
    productOfNumbersAdd(productOfNumbers, 3);
    productOfNumbersAdd(productOfNumbers, 4);
    productOfNumbersAdd(productOfNumbers, 5);
    runTest("Test Case 9", 60, productOfNumbersGetProduct(productOfNumbers, 3)); // 3*4*5=60

    // Test case 10
    productOfNumbersAdd(productOfNumbers, 0);
    runTest("Test Case 10", 0, productOfNumbersGetProduct(productOfNumbers, 2)); // 0

    productOfNumbersFree(productOfNumbers);

    return 0;
}

