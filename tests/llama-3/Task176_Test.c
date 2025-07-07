#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *products;
    int size;
} ProductOfNumbers;

ProductOfNumbers* productOfNumbersCreate() {
    ProductOfNumbers* obj = (ProductOfNumbers*)malloc(sizeof(ProductOfNumbers));
    obj->products = (int*)malloc(sizeof(int));
    obj->products[0] = 1;
    obj->size = 1;
    return obj;
}

void add(ProductOfNumbers* obj, int num) {
    if (num == 0) {
        free(obj->products);
        obj->products = (int*)malloc(sizeof(int));
        obj->products[0] = 1;
        obj->size = 1;
    } else {
        obj->products = (int*)realloc(obj->products, (obj->size + 1) * sizeof(int));
        obj->products[obj->size] = obj->products[obj->size - 1] * num;
        obj->size++;
    }
}

int getProduct(ProductOfNumbers* obj, int k) {
    if (k >= obj->size) {
        return 0;
    } else {
        return obj->products[obj->size - 1] / obj->products[obj->size - k - 1];
    }
}

void productOfNumbersFree(ProductOfNumbers* obj) {
    free(obj->products);
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

    add(productOfNumbers, 3);
    add(productOfNumbers, 0);
    add(productOfNumbers, 2);
    add(productOfNumbers, 5);
    add(productOfNumbers, 4);

    runTest("Test Case 1", 20, getProduct(productOfNumbers, 2));  // 5*4=20
    runTest("Test Case 2", 40, getProduct(productOfNumbers, 3));  // 2*5*4=40
    runTest("Test Case 3", 0, getProduct(productOfNumbers, 4));   // 0*2*5*4=0

    add(productOfNumbers, 8);
    runTest("Test Case 4", 32, getProduct(productOfNumbers, 2));  // 4*8=32

    add(productOfNumbers, 6);
    runTest("Test Case 5", 192, getProduct(productOfNumbers, 3)); // 8*6*4=192

    productOfNumbersFree(productOfNumbers);

    // Test case 6
    productOfNumbers = productOfNumbersCreate();
    add(productOfNumbers, 5);
    runTest("Test Case 6", 5, getProduct(productOfNumbers, 1));   // 5

    add(productOfNumbers, 0);
    runTest("Test Case 7", 0, getProduct(productOfNumbers, 1));   // 0

    add(productOfNumbers, 1);
    add(productOfNumbers, 2);
    add(productOfNumbers, 3);
    add(productOfNumbers, 4);
    runTest("Test Case 8", 24, getProduct(productOfNumbers, 4));  // 1*2*3*4=24

    productOfNumbersFree(productOfNumbers);

    productOfNumbers = productOfNumbersCreate();
    add(productOfNumbers, 1);
    add(productOfNumbers, 2);
    add(productOfNumbers, 3);
    add(productOfNumbers, 4);
    add(productOfNumbers, 5);
    runTest("Test Case 9", 60, getProduct(productOfNumbers, 3));  // 3*4*5=60

    add(productOfNumbers, 0);
    runTest("Test Case 10", 0, getProduct(productOfNumbers, 2));  // 0

    productOfNumbersFree(productOfNumbers);

    return 0;
}
