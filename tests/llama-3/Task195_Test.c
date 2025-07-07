#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int size;
    int capacity;
    int *arr;
} Heap;

Heap* createHeap(int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->arr = (int*)malloc(sizeof(int) * capacity);
    return heap;
}

void heapifyUp(Heap* heap, int index) {
    if (index && heap->arr[index] > heap->arr[(index - 1) / 2]) {
        int temp = heap->arr[index];
        heap->arr[index] = heap->arr[(index - 1) / 2];
        heap->arr[(index - 1) / 2] = temp;
        heapifyUp(heap, (index - 1) / 2);
    }
}

void heapifyDown(Heap* heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;
    if (left < heap->size && heap->arr[left] > heap->arr[largest]) {
        largest = left;
    }
    if (right < heap->size && heap->arr[right] > heap->arr[largest]) {
        largest = right;
    }
    if (largest != index) {
        int temp = heap->arr[index];
        heap->arr[index] = heap->arr[largest];
        heap->arr[largest] = temp;
        heapifyDown(heap, largest);
    }
}

void insert(Heap* heap, int num) {
    if (heap->size == heap->capacity) {
        printf("Heap is full\n");
        return;
    }
    heap->arr[heap->size] = num;
    heap->size++;
    heapifyUp(heap, heap->size - 1);
}

int extractMax(Heap* heap) {
    if (heap->size == 0) {
        printf("Heap is empty\n");
        return -1;
    }
    int max = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return max;
}

typedef struct {
    Heap* lower;
    Heap* higher;
} MedianFinder;

MedianFinder* medianFinderCreate() {
    MedianFinder* medianFinder = (MedianFinder*)malloc(sizeof(MedianFinder));
    medianFinder->lower = createHeap(100001);
    medianFinder->higher = createHeap(100001);
    return medianFinder;
}

void addNum(MedianFinder* obj, int num) {
    if (obj->lower->size == 0 || num <= obj->lower->arr[0]) {
        insert(obj->lower, num);
    } else {
        insert(obj->higher, num);
    }

    if (obj->lower->size > obj->higher->size + 1) {
        insert(obj->higher, extractMax(obj->lower));
    } else if (obj->higher->size > obj->lower->size) {
        insert(obj->lower, extractMax(obj->higher));
    }
}

double findMedian(MedianFinder* obj) {
    if (obj->lower->size == obj->higher->size) {
        return (obj->lower->arr[0] + obj->higher->arr[0]) / 2.0;
    } else {
        return obj->lower->arr[0];
    }
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

    free(medianFinder->lower->arr);
    free(medianFinder->higher->arr);
    free(medianFinder->lower);
    free(medianFinder->higher);
    free(medianFinder);

    return 0;
}
