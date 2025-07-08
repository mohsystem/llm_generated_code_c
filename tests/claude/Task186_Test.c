#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy;
    struct ListNode* curr = &dummy;
    dummy.next = NULL;

    while (l1 && l2) {
        if (l1->val <= l2->val) {
            curr->next = l1;
            l1 = l1->next;
        } else {
            curr->next = l2;
            l2 = l2->next;
        }
        curr = curr->next;
    }

    curr->next = l1 ? l1 : l2;
    return dummy.next;
}

struct ListNode* mergeKListsHelper(struct ListNode** lists, int start, int end) {
    if (start == end) {
        return lists[start];
    }
    if (start + 1 == end) {
        return mergeTwoLists(lists[start], lists[end]);
    }

    int mid = start + (end - start) / 2;
    struct ListNode* left = mergeKListsHelper(lists, start, mid);
    struct ListNode* right = mergeKListsHelper(lists, mid + 1, end);
    return mergeTwoLists(left, right);
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    if (listsSize == 0) {
        return NULL;
    }

    return mergeKListsHelper(lists, 0, listsSize - 1);
}

// Helper to create a linked list from array
struct ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* current = head;
    for (int i = 1; i < size; i++) {
        struct ListNode* node = malloc(sizeof(struct ListNode));
        node->val = arr[i];
        node->next = NULL;
        current->next = node;
        current = node;
    }
    return head;
}

// Helper to free list memory
void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp);
    }
}

// Helper to compare linked list to expected array
int compareListToArray(struct ListNode* head, int* arr, int size) {
    for (int i = 0; i < size; i++) {
        if (!head || head->val != arr[i]) return 0;
        head = head->next;
    }
    return head == NULL;
}

// Test case runner
void runTest(struct ListNode** lists, int listsSize, int* expected, int expectedSize, int testNumber) {
    struct ListNode* result = mergeKLists(lists, listsSize);
    int pass = compareListToArray(result, expected, expectedSize);
    printf("Test %d: %s\n", testNumber, pass ? "pass" : "fail");
    freeList(result);
}

int main() {
    // Test case 1: [[1,4,5],[1,3,4],[2,6]] => [1,1,2,3,4,4,5,6]
    int arr1a[] = {1,4,5};
    int arr1b[] = {1,3,4};
    int arr1c[] = {2,6};
    struct ListNode* lists1[] = {createList(arr1a,3), createList(arr1b,3), createList(arr1c,2)};
    int expected1[] = {1,1,2,3,4,4,5,6};
    runTest(lists1, 3, expected1, 8, 1);



    // Test case 2: [] => []
    struct ListNode** lists2 = NULL;
    int expected2[] = {};
    runTest(lists2, 0, expected2, 0, 2);

    // Test case 3: [[]] => []
    struct ListNode* emptyList = NULL;
    struct ListNode* lists3[] = {emptyList};
    int expected3[] = {};
    runTest(lists3, 1, expected3, 0, 3);

    return 0;
}
