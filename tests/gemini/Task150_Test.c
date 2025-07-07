#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* newNode1(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

void insert(struct Node** head, int value) {
    struct Node* newNode = newNode1(value);
    newNode->next = *head;
    *head = newNode;
}

void deleteNode(struct Node** head, int value) {
    struct Node* current = *head;
    struct Node* previous = NULL;

    while (current != NULL && current->data != value) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return; // silently return if value not found
    }

    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
}

int search(struct Node* head, int value) {
    struct Node* current = head;
    while (current != NULL) {
        if (current->data == value) return 1;
        current = current->next;
    }
    return 0;
}

void freeList(struct Node** head) {
    struct Node* current = *head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

void run_tests() {
    struct Node* head = NULL;

    // Insert elements: list will be 30 -> 20 -> 10
    insert(&head, 10);
    insert(&head, 20);
    insert(&head, 30);

    // Test search found
    assert(search(head, 20) == 1);
    // Test search not found
    assert(search(head, 999) == 0);

    // Delete node 20
    deleteNode(&head, 20);
    assert(search(head, 20) == 0);

    // Check head data (should be 30 because insert adds at front)
    assert(head != NULL && head->data == 30);
    // Next node data should be 10
    assert(head->next != NULL && head->next->data == 10);

    // Delete non-existent value: should not crash or alter list
    deleteNode(&head, 999);
    assert(search(head, 999) == 0);

    // Free list and check head NULL
    freeList(&head);
    assert(head == NULL);

    // Search in empty list
    assert(search(head, 10) == 0);

    printf("✓ All tests passed for linked list implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
