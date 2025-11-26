#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50000

int arr[MAX_SIZE];
int count = 0;

// Comparison function for qsort to sort integers in ascending order
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Add a number to the data structure
void addNum(int num) {
    if (count < MAX_SIZE) {
        arr[count++] = num;
        qsort(arr, count, sizeof(int), compare); // Sort after adding each element
    } else {
        // Handle overflow if needed
    }
}

// Find the median of all numbers so far
double findMedian() {
    if (count == 0) {
        return 0.0; // Or handle the empty case differently
    } else if (count % 2 == 0) {
        // If count is even, median is the average of the two middle values
        return (double)(arr[count / 2 - 1] + arr[count / 2]) / 2;
    } else {
        // If count is odd, median is the middle value
        return (double)arr[count / 2];
    }
}

// Function to check if the computed median matches the expected value
void checkMedian(double expected, double actual) {
    if (fabs(expected - actual) < 1e-5) {
        printf("Pass\n");
    } else {
        printf("Fail\n");
    }
}

// Main function for testing the implementation
int main() {
    // Test case 1: Add 1, find median
    addNum(1);
    double result = findMedian();
    printf("Expected: 1.0, Result: %.5f -> ", result);
    checkMedian(1.0, result);

    // Test case 2: Add 2, find median
    addNum(2);
    result = findMedian();
    printf("Expected: 1.5, Result: %.5f -> ", result);
    checkMedian(1.5, result);

    // Test case 3: Add 3, find median
    addNum(3);
    result = findMedian();
    printf("Expected: 2.0, Result: %.5f -> ", result);
    checkMedian(2.0, result);

    // Test case 4: Add 3, 1, 4, 2, 5, find median
    addNum(3);
    addNum(1);
    addNum(4);
    addNum(2);
    addNum(5);
    result = findMedian();
    printf("Expected: 3.0, Result: %.5f -> ", result);
    checkMedian(3.0, result);

    // Test case 5: Add numbers, find median
    addNum(10);
    addNum(5);
    addNum(15);
    addNum(20);
    addNum(25);
    addNum(30);
    result = findMedian();
    printf("Expected: 17.5, Result: %.5f -> ", result);
    checkMedian(17.5, result);

    // Test case 6: Add 0 three times, find median
    addNum(0);
    addNum(0);
    addNum(0);
    result = findMedian();
    printf("Expected: 0.0, Result: %.5f -> ", result);
    checkMedian(0.0, result);

    // Test case 7: Add -1, 0, 1, find median
    addNum(-1);
    addNum(0);
    addNum(1);
    result = findMedian();
    printf("Expected: 0.0, Result: %.5f -> ", result);
    checkMedian(0.0, result);

    // Test case 8: Add negative numbers, find median
    addNum(-10);
    addNum(-20);
    addNum(-30);
    addNum(-40);
    addNum(-50);
    result = findMedian();
    printf("Expected: -30.0, Result: %.5f -> ", result);
    checkMedian(-30.0, result);

    // Test case 9: Add larger numbers, find median
    addNum(100);
    addNum(200);
    addNum(300);
    addNum(400);
    addNum(500);
    addNum(600);
    addNum(700);
    result = findMedian();
    printf("Expected: 400.0, Result: %.5f -> ", result);
    checkMedian(400.0, result);

    // Test case 10: Add 1 through 9, find median
    addNum(1);
    addNum(3);
    addNum(2);
    addNum(4);
    addNum(5);
    addNum(6);
    addNum(7);
    addNum(8);
    addNum(9);
    result = findMedian();
    printf("Expected: 5.0, Result: %.5f -> ", result);
    checkMedian(5.0, result);

    return 0;
}
