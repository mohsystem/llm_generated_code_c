#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct SinglyLinkedList {
    Node* head;
} SinglyLinkedList;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Memory error\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insert(SinglyLinkedList* list, int data) {
    if (list->head == NULL) {
        list->head = createNode(data);
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = createNode(data);
    }
}

void deleteNode(SinglyLinkedList* list, int data) {
    if (list->head == NULL) {
        return;
    }
    if (list->head->data == data) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
        return;
    }
    Node* current = list->head;
    while (current->next != NULL) {
        if (current->next->data == data) {
            Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
            return;
        }
        current = current->next;
    }
}

int search(SinglyLinkedList* list, int data) {
    Node* current = list->head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void freeList(SinglyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

void run_tests() {
    SinglyLinkedList list;
    list.head = NULL;

    // test empty list
    assert(list.head == NULL);

    // test insert
    insert(&list, 10);
    insert(&list, 20);
    assert(list.head != NULL);
    assert(list.head->data == 10);
    assert(list.head->next != NULL);
    assert(list.head->next->data == 20);

    // test insert multiple
    insert(&list, 30);
    insert(&list, 40);
    assert(list.head->next->next->data == 30);
    assert(list.head->next->next->next->data == 40);

    // test search found
    assert(search(&list, 20) == 1);
    // test search not found
    assert(search(&list, 999) == 0);

    // test delete non-existent
    deleteNode(&list, 999); // should not change list
    assert(list.head->data == 10);
    assert(list.head->next->data == 20);

    // test delete head
    deleteNode(&list, 10);
    assert(list.head->data == 20);

    // test delete last element
    deleteNode(&list, 40);
    Node* current = list.head;
    while (current->next != NULL) {
        current = current->next;
    }
    assert(current->data == 30);
    assert(current->next == NULL);

    // test search empty list after deletions
    freeList(&list);
    assert(list.head == NULL);
    assert(search(&list, 10) == 0);

    printf("✓ All tests passed for singly linked list implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
