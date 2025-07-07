#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    int* nums;
    int size;
    int capacity;
} MedianFinder;

// Create a new MedianFinder object
MedianFinder* medianFinderCreate() {
    MedianFinder* obj = (MedianFinder*)malloc(sizeof(MedianFinder));
    obj->nums = (int*)malloc(sizeof(int) * 50000);
    obj->size = 0;
    obj->capacity = 50000;
    return obj;
}

// Add a number to the data structure
void medianFinderAddNum(MedianFinder* obj, int num) {
    if (obj->size == obj->capacity) {
        obj->capacity *= 2;
        obj->nums = (int*)realloc(obj->nums, sizeof(int) * obj->capacity);
    }

    int i = obj->size - 1;
    while (i >= 0 && obj->nums[i] > num) {
        obj->nums[i + 1] = obj->nums[i];
        i--;
    }
    obj->nums[i + 1] = num;
    obj->size++;
}

// Find the median of all numbers so far
double medianFinderFindMedian(MedianFinder* obj) {
    if (obj->size % 2 == 0) {
        return (obj->nums[obj->size / 2 - 1] + obj->nums[obj->size / 2]) / 2.0;
    } else {
        return obj->nums[obj->size / 2];
    }
}

// Free the memory allocated for the MedianFinder object
void medianFinderFree(MedianFinder* obj) {
    free(obj->nums);
    free(obj);
}

// Function to check whether the result is correct
void checkMedian(double expected, double actual) {
    if (fabs(expected - actual) < 1e-5) {
        printf("Pass\n");
    } else {
        printf("Fail\n");
    }
}

// Main function for testing the implementation
int main() {
    // Define the words list
    char* words[] = {"acckzz", "ccbazz", "eiowzz", "abcczz"};
    int wordsSize = 4;

    // Create the MedianFinder object
    MedianFinder* obj = medianFinderCreate();

    // Test case 1: Add 1, find median
    medianFinderAddNum(obj, 1);
    double result = medianFinderFindMedian(obj);
    printf("Expected: 1.0, Result: %.5f -> ", result);
    checkMedian(1.0, result);

    // Test case 2: Add 2, find median
    medianFinderAddNum(obj, 2);
    result = medianFinderFindMedian(obj);
    printf("Expected: 1.5, Result: %.5f -> ", result);
    checkMedian(1.5, result);

    // Test case 3: Add 3, find median
    medianFinderAddNum(obj, 3);
    result = medianFinderFindMedian(obj);
    printf("Expected: 2.0, Result: %.5f -> ", result);
    checkMedian(2.0, result);

    // Test case 4: Add 3, 1, 4, 2, 5, find median
    medianFinderAddNum(obj, 3);
    medianFinderAddNum(obj, 1);
    medianFinderAddNum(obj, 4);
    medianFinderAddNum(obj, 2);
    medianFinderAddNum(obj, 5);
    result = medianFinderFindMedian(obj);
    printf("Expected: 3.0, Result: %.5f -> ", result);
    checkMedian(3.0, result);

    // Test case 5: Add numbers, find median
    medianFinderAddNum(obj, 10);
    medianFinderAddNum(obj, 5);
    medianFinderAddNum(obj, 15);
    medianFinderAddNum(obj, 20);
    medianFinderAddNum(obj, 25);
    medianFinderAddNum(obj, 30);
    result = medianFinderFindMedian(obj);
    printf("Expected: 17.5, Result: %.5f -> ", result);
    checkMedian(17.5, result);

    // Test case 6: Add 0 three times, find median
    medianFinderAddNum(obj, 0);
    medianFinderAddNum(obj, 0);
    medianFinderAddNum(obj, 0);
    result = medianFinderFindMedian(obj);
    printf("Expected: 0.0, Result: %.5f -> ", result);
    checkMedian(0.0, result);

    // Test case 7: Add -1, 0, 1, find median
    medianFinderAddNum(obj, -1);
    medianFinderAddNum(obj, 0);
    medianFinderAddNum(obj, 1);
    result = medianFinderFindMedian(obj);
    printf("Expected: 0.0, Result: %.5f -> ", result);
    checkMedian(0.0, result);

    // Test case 8: Add negative numbers, find median
    medianFinderAddNum(obj, -10);
    medianFinderAddNum(obj, -20);
    medianFinderAddNum(obj, -30);
    medianFinderAddNum(obj, -40);
    medianFinderAddNum(obj, -50);
    result = medianFinderFindMedian(obj);
    printf("Expected: -30.0, Result: %.5f -> ", result);
    checkMedian(-30.0, result);

    // Test case 9: Add larger numbers, find median
    medianFinderAddNum(obj, 100);
    medianFinderAddNum(obj, 200);
    medianFinderAddNum(obj, 300);
    medianFinderAddNum(obj, 400);
    medianFinderAddNum(obj, 500);
    medianFinderAddNum(obj, 600);
    medianFinderAddNum(obj, 700);
    result = medianFinderFindMedian(obj);
    printf("Expected: 400.0, Result: %.5f -> ", result);
    checkMedian(400.0, result);

    // Test case 10: Add 1 through 9, find median
    medianFinderAddNum(obj, 1);
    medianFinderAddNum(obj, 3);
    medianFinderAddNum(obj, 2);
    medianFinderAddNum(obj, 4);
    medianFinderAddNum(obj, 5);
    medianFinderAddNum(obj, 6);
    medianFinderAddNum(obj, 7);
    medianFinderAddNum(obj, 8);
    medianFinderAddNum(obj, 9);
    result = medianFinderFindMedian(obj);
    printf("Expected: 5.0, Result: %.5f -> ", result);
    checkMedian(5.0, result);

    // Free the memory allocated for the object
    medianFinderFree(obj);

    return 0;
}
