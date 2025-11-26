#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct Node {
    int key;
    struct Node *left, *right;
};

struct Node* newNode(int item) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

struct Node* insert(struct Node* node, int key) {
    if (node == NULL) return newNode(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    return node;
}

struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

struct Node* deleteNode(struct Node* root, int key) {
    if (root == NULL) return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // node with only one child or no child
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }
        // node with two children: get inorder successor (smallest in right subtree)
        struct Node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

bool search(struct Node* root, int key) {
    if (root == NULL) return false;
    if (root->key == key) return true;
    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

void freeTree(struct Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Test runner with all test cases
void run_tests() {
    struct Node* root = NULL;

    // Test 1: insert and search
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    assert(search(root, 10) == true);
    assert(search(root, 20) == true);
    assert(search(root, 5) == true);
    assert(search(root, 15) == false);

    // Test 2: delete leaf node
    root = insert(root, 25);
    root = deleteNode(root, 25);
    assert(search(root, 25) == false);

    // Test 3: delete node with one child
    root = insert(root, 6);
    root = deleteNode(root, 5);
    assert(search(root, 5) == false);
    assert(search(root, 6) == true);

    // Test 4: delete node with two children
    root = insert(root, 15);
    root = insert(root, 12);
    root = deleteNode(root, 20);
    assert(search(root, 20) == false);
    assert(search(root, 12) == true);

    // Test 5: delete root node
    root = deleteNode(root, 10);
    assert(search(root, 10) == false);
    assert(search(root, 6) == true);
    assert(search(root, 15) == true);

    // Test 6: search empty tree
    struct Node* emptyRoot = NULL;
    assert(search(emptyRoot, 10) == false);

    // Test 7: insert multiple nodes
    root = insert(root, 7);
    root = insert(root, 11);
    assert(search(root, 7) == true);
    assert(search(root, 11) == true);

    // Test 8: delete node with no children
    root = deleteNode(root, 7);
    assert(search(root, 7) == false);

    // Test 9: delete node with left child
    root = insert(root, 8);
    root = insert(root, 6);
    root = deleteNode(root, 11);
    assert(search(root, 11) == false);
    assert(search(root, 8) == true);
    assert(search(root, 6) == true);

    // Test 10: delete node with right child
    root = insert(root, 13);
    root = insert(root, 14);
    root = deleteNode(root, 13);
    assert(search(root, 13) == false);
    assert(search(root, 14) == true);

    freeTree(root);

    printf("✓ All test cases passed for Binary Search Tree implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
