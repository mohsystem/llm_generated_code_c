#include <stdio.h>
#include <stdlib.h>

struct ListNode {
  int val;
  struct ListNode *next;
};

struct ListNode *mergeTwoLists(struct ListNode *l1, struct ListNode *l2) {
  if (l1 == NULL) return l2;
  if (l2 == NULL) return l1;
  if (l1->val < l2->val) {
    l1->next = mergeTwoLists(l1->next, l2);
    return l1;
  } else {
    l2->next = mergeTwoLists(l1, l2->next);
    return l2;
  }
}

struct ListNode *mergeKLists(struct ListNode **lists, int k) {
  if (k == 0) return NULL;
  int i;
  struct ListNode *result = lists[0];
  for (i = 1; i < k; i++) {
    result = mergeTwoLists(result, lists[i]);
  }
  return result;
}

// Utility: create linked list from array
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

// Utility: convert list to array, returns length
int listToArray(struct ListNode* head, int* outArr, int maxSize) {
  int count = 0;
  while (head && count < maxSize) {
    outArr[count++] = head->val;
    head = head->next;
  }
  return count;
}

// Utility: free linked list
void freeList(struct ListNode* head) {
  while (head) {
    struct ListNode* tmp = head;
    head = head->next;
    free(tmp);
  }
}

// Compare two arrays for equality
int arraysEqual(int* a, int sizeA, int* b, int sizeB) {
  if (sizeA != sizeB) return 0;
  for (int i = 0; i < sizeA; i++) {
    if (a[i] != b[i]) return 0;
  }
  return 1;
}

void runTest(int testNumber, struct ListNode** lists, int k, int* expected, int expectedSize) {
  struct ListNode* result = mergeKLists(lists, k);
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
  // Test case 1: lists = [[1,4,5],[1,3,4],[2,6]]
  int arr1[] = {1,4,5};
  int arr2[] = {1,3,4};
  int arr3[] = {2,6};
  struct ListNode* l1 = createList(arr1, 3);
  struct ListNode* l2 = createList(arr2, 3);
  struct ListNode* l3 = createList(arr3, 2);
  struct ListNode* lists1[] = {l1, l2, l3};
  int expected1[] = {1,1,2,3,4,4,5,6};
  runTest(1, lists1, 3, expected1, 8);


  // Test case 2: lists = []
  struct ListNode** lists2 = NULL;
  int expected2[] = {};
  runTest(2, lists2, 0, expected2, 0);

  // Test case 3: lists = [[]]
  struct ListNode* emptyList = NULL;
  struct ListNode* lists3[] = {emptyList};
  int expected3[] = {};
  runTest(3, lists3, 1, expected3, 0);

  return 0;
}
