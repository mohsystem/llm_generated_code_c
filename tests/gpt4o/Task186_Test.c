#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    int* heap = (int*)malloc(100000 * sizeof(int));
    int heapSize = 0;

    for (int i = 0; i < listsSize; ++i) {
        struct ListNode* l = lists[i];
        while (l) {
            heap[heapSize++] = l->val;
            l = l->next;
        }
    }

    // Correct comparator for integers
    int compare(const void* a, const void* b) {
        int int_a = *(int*)a;
        int int_b = *(int*)b;
        if (int_a < int_b) return -1;
        else if (int_a > int_b) return 1;
        else return 0;
    }

    qsort(heap, heapSize, sizeof(int), compare);

    struct ListNode* dummy = createNode(0);
    struct ListNode* curr = dummy;
    for (int i = 0; i < heapSize; ++i) {
        curr->next = createNode(heap[i]);
        curr = curr->next;
    }

    free(heap);
    return dummy->next;
}

void printList(struct ListNode* node) {
    while (node) {
        printf("%d ", node->val);
        node = node->next;
    }
    printf("\n");
}

// Convert linked list to array for comparison
int listToArray(struct ListNode* head, int* arr, int maxSize) {
    int count = 0;
    while (head && count < maxSize) {
        arr[count++] = head->val;
        head = head->next;
    }
    return count;
}

// Free linked list memory
void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp);
    }
}

// Compare arrays
int arraysEqual(int* a, int sizeA, int* b, int sizeB) {
    if (sizeA != sizeB) return 0;
    for (int i = 0; i < sizeA; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

// Helper to create list from array
struct ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = createNode(arr[0]);
    struct ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = createNode(arr[i]);
        curr = curr->next;
    }
    return head;
}

void runTest(int testNumber, struct ListNode** lists, int listsSize, int* expected, int expectedSize) {
    struct ListNode* merged = mergeKLists(lists, listsSize);
    int actual[10000];
    int actualSize = listToArray(merged, actual, 10000);

    if (arraysEqual(actual, actualSize, expected, expectedSize)) {
        printf("Test %d: pass\n", testNumber);
    } else {
        printf("Test %d: fail\nExpected: ", testNumber);
        for (int i = 0; i < expectedSize; i++) printf("%d ", expected[i]);
        printf("\nGot:      ");
        for (int i = 0; i < actualSize; i++) printf("%d ", actual[i]);
        printf("\n");
    }
    freeList(merged);
}

int main() {
    // Test 1: [[1,4,5],[1,3,4],[2,6]]
    int arr1[] = {1,4,5};
    int arr2[] = {1,3,4};
    int arr3[] = {2,6};
    struct ListNode* l1 = createList(arr1, 3);
    struct ListNode* l2 = createList(arr2, 3);
    struct ListNode* l3 = createList(arr3, 2);
    struct ListNode* lists1[] = {l1, l2, l3};
    int expected1[] = {1,1,2,3,4,4,5,6};
    runTest(1, lists1, 3, expected1, 8);
    freeList(l1);
    freeList(l2);
    freeList(l3);

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
