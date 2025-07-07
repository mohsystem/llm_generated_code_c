#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode* next;
} ListNode;

void swap(ListNode** a, ListNode** b) {
    ListNode* temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(ListNode** arr, int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left]->val < arr[smallest]->val) {
        smallest = left;
    }

    if (right < n && arr[right]->val < arr[smallest]->val) {
        smallest = right;
    }

    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        heapify(arr, n, smallest);
    }
}

void buildHeap(ListNode** arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

ListNode* mergeKLists(ListNode** lists, int k) {
    if (k == 0) return NULL;

    ListNode* dummy = (ListNode*)malloc(sizeof(ListNode));
    dummy->next = NULL;
    ListNode* curr = dummy;

    // Create an array with non-null list heads
    ListNode** arr = (ListNode**)malloc(k * sizeof(ListNode*));
    int size = 0;
    for (int i = 0; i < k; i++) {
        if (lists[i] != NULL) {
            arr[size++] = lists[i];
        }
    }

    if (size == 0) {
        free(arr);
        free(dummy);
        return NULL;
    }

    buildHeap(arr, size);

    while (size > 0) {
        curr->next = arr[0];
        curr = curr->next;

        if (arr[0]->next != NULL) {
            arr[0] = arr[0]->next;
        } else {
            arr[0] = arr[size - 1];
            size--;
        }
        heapify(arr, size, 0);
    }

    free(arr);
    ListNode* result = dummy->next;
    free(dummy);
    return result;
}

// --- Helper functions ---

// Create linked list from array
ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;
    ListNode* head = (ListNode*)malloc(sizeof(ListNode));
    head->val = arr[0];
    head->next = NULL;
    ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = (ListNode*)malloc(sizeof(ListNode));
        curr = curr->next;
        curr->val = arr[i];
        curr->next = NULL;
    }
    return head;
}

// Convert linked list to array
int listToArray(ListNode* head, int* arr, int maxSize) {
    int count = 0;
    while (head && count < maxSize) {
        arr[count++] = head->val;
        head = head->next;
    }
    return count;
}

// Free linked list memory
void freeList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// Compare two integer arrays
int arraysEqual(int* a, int sizeA, int* b, int sizeB) {
    if (sizeA != sizeB) return 0;
    for (int i = 0; i < sizeA; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void runTest(int testNumber, ListNode** lists, int listsSize, int* expected, int expectedSize) {
    ListNode* merged = mergeKLists(lists, listsSize);
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
    ListNode* l1 = createList(arr1, 3);
    ListNode* l2 = createList(arr2, 3);
    ListNode* l3 = createList(arr3, 2);
    ListNode* lists1[] = {l1, l2, l3};
    int expected1[] = {1,1,2,3,4,4,5,6};
    runTest(1, lists1, 3, expected1, 8);


    // Test 2: []
    ListNode** lists2 = NULL;
    int expected2[] = {};
    runTest(2, lists2, 0, expected2, 0);

    // Test 3: [[]]
    ListNode* emptyList = NULL;
    ListNode* lists3[] = {emptyList};
    int expected3[] = {};
    runTest(3, lists3, 1, expected3, 0);

    return 0;
}
