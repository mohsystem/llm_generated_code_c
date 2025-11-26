#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int* nums;
    int size;
} MedianFinder;

MedianFinder* medianFinderCreate() {
    MedianFinder* obj = (MedianFinder*)malloc(sizeof(MedianFinder));
    obj->nums = NULL;
    obj->size = 0;
    return obj;
}

void addNum(MedianFinder* obj, int num) {
    obj->nums = (int*)realloc(obj->nums, (obj->size + 1) * sizeof(int));
    int i = obj->size - 1;
    while (i >= 0 && obj->nums[i] > num) {
        obj->nums[i + 1] = obj->nums[i];
        i--;
    }
    obj->nums[i + 1] = num;
    obj->size++;
}

double findMedian(MedianFinder* obj) {
    if (obj->size % 2 == 1) {
        return obj->nums[obj->size / 2];
    } else {
        return (obj->nums[obj->size / 2 - 1] + obj->nums[obj->size / 2]) / 2.0;
    }
}

void medianFinderFree(MedianFinder* obj) {
    free(obj->nums);
    free(obj);
}

int main() {
    MedianFinder* medianFinder = medianFinderCreate();
    int result;

    // Test Case 1
    addNum(medianFinder, 1);
    result = fabs(findMedian(medianFinder) - 1.0) < 1e-5 ? 1 : 0;
    printf("Test 1: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 2
    addNum(medianFinder, 2);
    result = fabs(findMedian(medianFinder) - 1.5) < 1e-5 ? 1 : 0;
    printf("Test 2: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 3
    addNum(medianFinder, 3);
    result = fabs(findMedian(medianFinder) - 2.0) < 1e-5 ? 1 : 0;
    printf("Test 3: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 4
    addNum(medianFinder, 3);
    addNum(medianFinder, 1);
    addNum(medianFinder, 4);
    addNum(medianFinder, 2);
    addNum(medianFinder, 5);
    result = fabs(findMedian(medianFinder) - 3.0) < 1e-5 ? 1 : 0;
    printf("Test 4: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 5
    addNum(medianFinder, 10);
    addNum(medianFinder, 5);
    addNum(medianFinder, 15);
    addNum(medianFinder, 20);
    addNum(medianFinder, 25);
    addNum(medianFinder, 30);
    result = fabs(findMedian(medianFinder) - 17.5) < 1e-5 ? 1 : 0;
    printf("Test 5: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 6
    addNum(medianFinder, 0);
    addNum(medianFinder, 0);
    addNum(medianFinder, 0);
    result = fabs(findMedian(medianFinder) - 0.0) < 1e-5 ? 1 : 0;
    printf("Test 6: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 7
    addNum(medianFinder, -1);
    addNum(medianFinder, 0);
    addNum(medianFinder, 1);
    result = fabs(findMedian(medianFinder) - 0.0) < 1e-5 ? 1 : 0;
    printf("Test 7: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 8
    addNum(medianFinder, -10);
    addNum(medianFinder, -20);
    addNum(medianFinder, -30);
    addNum(medianFinder, -40);
    addNum(medianFinder, -50);
    result = fabs(findMedian(medianFinder) - -30.0) < 1e-5 ? 1 : 0;
    printf("Test 8: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 9
    addNum(medianFinder, 100);
    addNum(medianFinder, 200);
    addNum(medianFinder, 300);
    addNum(medianFinder, 400);
    addNum(medianFinder, 500);
    addNum(medianFinder, 600);
    addNum(medianFinder, 700);
    result = fabs(findMedian(medianFinder) - 400.0) < 1e-5 ? 1 : 0;
    printf("Test 9: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    // Test Case 10
    addNum(medianFinder, 1);
    addNum(medianFinder, 3);
    addNum(medianFinder, 2);
    addNum(medianFinder, 4);
    addNum(medianFinder, 5);
    addNum(medianFinder, 6);
    addNum(medianFinder, 7);
    addNum(medianFinder, 8);
    addNum(medianFinder, 9);
    result = fabs(findMedian(medianFinder) - 5.0) < 1e-5 ? 1 : 0;
    printf("Test 10: %.1f - %s\n", findMedian(medianFinder), result ? "Pass" : "Fail");

    medianFinderFree(medianFinder);
    return 0;
}
