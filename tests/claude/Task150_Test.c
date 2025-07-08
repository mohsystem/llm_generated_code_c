#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insert(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = newNode;
    }
}

void delete(Node** head, int data) {
    if (*head == NULL) return;
    if ((*head)->data == data) {
        Node* temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }
    Node* current = *head;
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

bool search(Node* head, int data) {
    Node* current = head;
    while (current != NULL) {
        if (current->data == data) return true;
        current = current->next;
    }
    return false;
}

void freeList(Node** head) {
    Node* current = *head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

void run_tests() {
    Node* head = NULL;

    // test_empty_list
    assert(head == NULL);
    // test_insert
    insert(&head, 10);
    insert(&head, 20);
    assert(head != NULL);
    assert(head->data == 10);
    assert(head->next != NULL);
    assert(head->next->data == 20);

    // test_insert_multiple
    insert(&head, 30);
    insert(&head, 40);
    assert(head->next->next->data == 30);
    assert(head->next->next->next->data == 40);

    // test_search_found
    assert(search(head, 20) == true);
    // test_search_not_found
    assert(search(head, 999) == false);

    // test_delete_non_existent
    delete(&head, 999); // should not change list
    assert(head->data == 10);
    assert(head->next->data == 20);

    // test_delete_head
    delete(&head, 10);
    assert(head->data == 20);

    // test_delete_last_element
    delete(&head, 40);
    Node* current = head;
    while (current->next != NULL)
        current = current->next;
    assert(current->data == 30);
    assert(current->next == NULL);

    // free memory after tests
    freeList(&head);
    assert(head == NULL);

    // test_search_empty_list
    assert(search(head, 10) == false);

    printf("✓ All tests passed for singly linked list implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
