#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

void heapify(struct ListNode** heap, int i, int heapSize) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < heapSize && heap[left]->val < heap[smallest]->val) {
        smallest = left;
    }
    if (right < heapSize && heap[right]->val < heap[smallest]->val) {
        smallest = right;
    }
    if (smallest != i) {
        struct ListNode* temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        heapify(heap, smallest, heapSize);
    }
}

void buildHeap(struct ListNode** heap, int heapSize) {
    for (int i = heapSize / 2 - 1; i >= 0; i--) {
        heapify(heap, i, heapSize);
    }
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    if (listsSize == 0) return NULL;
    struct ListNode* dummy = (struct ListNode*) malloc(sizeof(struct ListNode));
    dummy->next = NULL;
    struct ListNode* curr = dummy;
    struct ListNode** heap = (struct ListNode**) malloc(sizeof(struct ListNode*) * listsSize);
    int heapSize = 0;
    for (int i = 0; i < listsSize; i++) {
        if (lists[i] != NULL) {
            heap[heapSize++] = lists[i];
        }
    }
    buildHeap(heap, heapSize);
    while (heapSize > 0) {
        curr->next = heap[0];
        curr = curr->next;
        if (curr->next != NULL) {
            heap[0] = curr->next;
        } else {
            heap[0] = heap[--heapSize];
        }
        heapify(heap, 0, heapSize);
    }
    struct ListNode* merged = dummy->next;
    free(dummy);
    free(heap);
    return merged;
}

// Helpers to create list from array
struct ListNode* createList(int* arr, int size) {
    struct ListNode* head = NULL;
    struct ListNode* curr = NULL;
    for (int i = 0; i < size; i++) {
        struct ListNode* node = (struct ListNode*) malloc(sizeof(struct ListNode));
        node->val = arr[i];
        node->next = NULL;
        if (!head) {
            head = node;
            curr = node;
        } else {
            curr->next = node;
            curr = node;
        }
    }
    return head;
}

// Helpers to convert list to array for comparison
int listToArray(struct ListNode* head, int* arr, int maxSize) {
    int count = 0;
    while (head && count < maxSize) {
        arr[count++] = head->val;
        head = head->next;
    }
    return count;
}

void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int arraysEqual(int* a, int sizeA, int* b, int sizeB) {
    if (sizeA != sizeB) return 0;
    for (int i = 0; i < sizeA; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void runTest(int testNumber, struct ListNode** lists, int listsSize, int* expected, int expectedSize) {
    struct ListNode* result = mergeKLists(lists, listsSize);
    int actual[10000];
    int actualSize = listToArray(result, actual, 10000);

    if (arraysEqual(actual, actualSize, expected, expectedSize)) {
        printf("Test %d: pass\n", testNumber);
    } else {
        printf("Test %d: fail\nExpected: ", testNumber);
        for (int i = 0; i < expectedSize; i++) printf("%d ", expected[i]);
        printf("\nGot:      ");
        for (int i = 0; i < actualSize; i++) printf("%d ", actual[i]);
        printf("\n");
    }
    freeList(result);
}

int main() {
    // Test 1: [[1,4,5],[1,3,4],[2,6]]
    int arr1[] = {1,4,5};
    int arr2[] = {1,3,4};
    int arr3[] = {2,6};
    struct ListNode* list1 = createList(arr1, 3);
    struct ListNode* list2 = createList(arr2, 3);
    struct ListNode* list3 = createList(arr3, 2);
    struct ListNode* lists1[] = {list1, list2, list3};
    int expected1[] = {1,1,2,3,4,4,5,6};
    runTest(1, lists1, 3, expected1, 8);



    // Test 2: []
    struct ListNode** lists2 = NULL;
    int expected2[] = {};
    runTest(2, lists2, 0, expected2, 0);

    // Test 3: [[]]
    struct ListNode* emptyList = NULL;
    struct ListNode* lists3[] = {emptyList};
    int expected3[] = {};
    runTest(3, lists3, 1, expected3, 0);

    return 0;
}
