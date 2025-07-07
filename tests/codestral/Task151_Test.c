#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct node {
    int key;
    struct node *left, *right;
};

struct node *newNode(int item) {
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

struct node* insert(struct node* node, int key) {
    if (node == NULL) return newNode(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    return node;
}

struct node *minValueNode(struct node* node) {
    struct node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

struct node* deleteNode(struct node* root, int key) {
    if (root == NULL) return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if (root->left == NULL) {
            struct node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct node *temp = root->left;
            free(root);
            return temp;
        }
        struct node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

int search(struct node* root, int key) {
    if (root == NULL || root->key == key)
       return root != NULL;
    if (root->key < key)
       return search(root->right, key);
    return search(root->left, key);
}

void freeTree(struct node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void run_tests() {
    struct node* root = NULL;

    // Insert and search
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    assert(search(root, 10) == 1);
    assert(search(root, 20) == 1);
    assert(search(root, 5) == 1);
    assert(search(root, 15) == 0);

    // Delete leaf node
    root = insert(root, 25);
    root = deleteNode(root, 25);
    assert(search(root, 25) == 0);

    // Delete node with one child
    root = insert(root, 6);
    root = deleteNode(root, 5);
    assert(search(root, 5) == 0);
    assert(search(root, 6) == 1);

    // Delete node with two children
    root = insert(root, 15);
    root = insert(root, 12);
    root = deleteNode(root, 20);
    assert(search(root, 20) == 0);
    assert(search(root, 12) == 1);

    // Delete root
    root = deleteNode(root, 10);
    assert(search(root, 10) == 0);
    assert(search(root, 6) == 1);
    assert(search(root, 15) == 1);

    // Search empty tree
    struct node* emptyRoot = NULL;
    assert(search(emptyRoot, 10) == 0);

    // Insert multiple nodes
    root = insert(root, 7);
    root = insert(root, 11);
    assert(search(root, 7) == 1);
    assert(search(root, 11) == 1);

    // Delete node with no children
    root = deleteNode(root, 7);
    assert(search(root, 7) == 0);

    // Delete node with left child
    root = insert(root, 8);
    root = insert(root, 6);
    root = deleteNode(root, 11);
    assert(search(root, 11) == 0);
    assert(search(root, 8) == 1);
    assert(search(root, 6) == 1);

    // Delete node with right child
    root = insert(root, 13);
    root = insert(root, 14);
    root = deleteNode(root, 13);
    assert(search(root, 13) == 0);
    assert(search(root, 14) == 1);

    freeTree(root);
    printf("✓ All tests passed for Binary Search Tree implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
