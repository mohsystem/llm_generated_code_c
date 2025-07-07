#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} SinglyLinkedList;

void initList(SinglyLinkedList* list) {
    list->head = NULL;
}

void insert(SinglyLinkedList* list, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (!list->head) {
        list->head = newNode;
    } else {
        Node* current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

int delete(SinglyLinkedList* list, int data) {
    Node* current = list->head;
    Node* prev = NULL;
    while (current && current->data != data) {
        prev = current;
        current = current->next;
    }
    if (!current) {
        return 0; // Not found
    }
    if (!prev) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    return 1; // Deleted successfully
}

int search(SinglyLinkedList* list, int data) {
    Node* current = list->head;
    while (current) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void freeList(SinglyLinkedList* list) {
    Node* current = list->head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        free(tmp);
    }
    list->head = NULL;
}

void run_tests() {
    SinglyLinkedList list;
    initList(&list);

    // empty list test
    assert(list.head == NULL);

    // insert test
    insert(&list, 10);
    insert(&list, 20);
    insert(&list, 30);
    assert(list.head != NULL);
    assert(list.head->data == 10);
    assert(list.head->next != NULL && list.head->next->data == 20);

    // search found
    assert(search(&list, 20) == 1);
    // search not found
    assert(search(&list, 999) == 0);

    // delete existing
    int deleted = delete(&list, 20);
    assert(deleted == 1);
    assert(search(&list, 20) == 0);

    // delete non-existent
    deleted = delete(&list, 999);
    assert(deleted == 0);

    // delete head
    deleted = delete(&list, 10);
    assert(deleted == 1);
    assert(list.head->data == 30);

    // delete last
    deleted = delete(&list, 30);
    assert(deleted == 1);
    assert(list.head == NULL);

    // search empty list
    assert(search(&list, 10) == 0);

    freeList(&list);

    printf("✓ All tests passed for singly linked list implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
