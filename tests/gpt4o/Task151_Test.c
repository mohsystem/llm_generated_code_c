#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int key;
    struct Node *left, *right;
};

struct Node* newNode(int item) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
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
        struct Node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

struct Node* search(struct Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    if (root->key < key)
        return search(root->right, key);
    return search(root->left, key);
}

// اختبار صارم
void runTests() {
    struct Node* root = NULL;

    // Insert nodes
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    // Test search
    assert(search(root, 50) != NULL);
    assert(search(root, 30) != NULL);
    assert(search(root, 70) != NULL);
    assert(search(root, 20) != NULL);
    assert(search(root, 40) != NULL);
    assert(search(root, 60) != NULL);
    assert(search(root, 80) != NULL);
    assert(search(root, 100) == NULL);

    // Test delete leaf node
    root = deleteNode(root, 20);
    assert(search(root, 20) == NULL);

    // Test delete node with one child
    root = deleteNode(root, 30);
    assert(search(root, 30) == NULL);
    assert(search(root, 40) != NULL);

    // Test delete node with two children
    root = deleteNode(root, 70);
    assert(search(root, 70) == NULL);
    assert(search(root, 60) != NULL);
    assert(search(root, 80) != NULL);

    // Test delete root node
    root = deleteNode(root, 50);
    assert(search(root, 50) == NULL);
    assert(search(root, 40) != NULL);

    printf("All BST test cases passed!\n");
}

int main() {
    runTests();
    return 0;
}
