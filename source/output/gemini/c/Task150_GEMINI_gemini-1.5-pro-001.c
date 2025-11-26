#include <stdio.h>
#include <stdlib.h>

// Node structure for the linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to create a new node
//error: called object ‘newNode’ is not a function or function pointer. we did rename only.LLM used the function name same as variable name.
struct Node*
    newNode1(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a new node at the beginning of the list
void insert(struct Node** head, int value) {
    struct Node* newNode = newNode1(value);
    newNode->next = *head;
    *head = newNode;
}

// Function to delete a node with a given value
void deleteNode(struct Node** head, int value) {
    struct Node* current = *head;
    struct Node* previous = NULL;

    while (current != NULL && current->data != value) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Value not found in the list.");
        return;
    }

    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
}

// Function to search for a node with a given value
int search(struct Node* head, int value) {
    struct Node* current = head;

    while (current != NULL) {
        if (current->data == value) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

// Function to print the linked list
void printList(struct Node* head) {
    struct Node* current = head;

    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }

    printf("");
}

int main() {
    struct Node* head = NULL;

    insert(&head, 10);
    insert(&head, 20);
    insert(&head, 30);

    printf("Linked list: ");
    printList(head);

    printf("Searching for 20: %d", search(head, 20));

    deleteNode(&head, 20);

    printf("Linked list after deleting 20: ");
    printList(head);

    return 0;
}