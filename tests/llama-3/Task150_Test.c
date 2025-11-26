#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int data;
    struct Node* next;
};

struct LinkedList {
    struct Node* head;
};

void insert(struct LinkedList* ll, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (ll->head == NULL) {
        ll->head = newNode;
    } else {
        struct Node* current = ll->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void deleteNode(struct LinkedList* ll, int data) {
    if (ll->head == NULL) {
        return;
    }

    if (ll->head->data == data) {
        struct Node* temp = ll->head;
        ll->head = ll->head->next;
        free(temp);
        return;
    }

    struct Node* current = ll->head;
    while (current->next != NULL) {
        if (current->next->data == data) {
            struct Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
            return;
        }
        current = current->next;
    }
}

int search(struct LinkedList* ll, int data) {
    struct Node* current = ll->head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void freeList(struct LinkedList* ll) {
    struct Node* current = ll->head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    ll->head = NULL;
}

void run_tests() {
    struct LinkedList ll;
    ll.head = NULL;

    // empty list test
    assert(ll.head == NULL);

    // insert test
    insert(&ll, 1);
    insert(&ll, 2);
    insert(&ll, 3);
    assert(ll.head != NULL);
    assert(ll.head->data == 1);
    assert(ll.head->next != NULL && ll.head->next->data == 2);
    assert(ll.head->next->next != NULL && ll.head->next->next->data == 3);

    // search found
    assert(search(&ll, 2) == 1);
    // search not found
    assert(search(&ll, 999) == 0);

    // delete node 2
    deleteNode(&ll, 2);
    assert(search(&ll, 2) == 0);

    // delete non-existent node should do nothing
    deleteNode(&ll, 999);
    assert(search(&ll, 1) == 1);
    assert(search(&ll, 3) == 1);

    // delete head node 1
    deleteNode(&ll, 1);
    assert(ll.head != NULL && ll.head->data == 3);

    // delete last remaining node 3
    deleteNode(&ll, 3);
    assert(ll.head == NULL);

    // search in empty list
    assert(search(&ll, 1) == 0);

    freeList(&ll);

    printf("✓ All tests passed for linked list implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
