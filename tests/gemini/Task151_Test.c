#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

int minValue(struct Node* node) {
    int minv = node->data;
    while (node->left != NULL) {
        node = node->left;
        minv = node->data;
    }
    return minv;
}

struct Node* insert(struct Node* node, int data) {
    if (node == NULL) {
        return newNode(data);
    }
    if (data < node->data) {
        node->left = insert(node->left, data);
    } else if (data > node->data) {
        node->right = insert(node->right, data);
    }
    return node;
}

struct Node* deleteNode(struct Node* node, int data) {
    if (node == NULL) {
        return node;
    }
    if (data < node->data) {
        node->left = deleteNode(node->left, data);
    } else if (data > node->data) {
        node->right = deleteNode(node->right, data);
    } else {
        if (node->left == NULL) {
            struct Node* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            struct Node* temp = node->left;
            free(node);
            return temp;
        }
        node->data = minValue(node->right);
        node->right = deleteNode(node->right, node->data);
    }
    return node;
}

bool search(struct Node* node, int data) {
    if (node == NULL) {
        return false;
    }
    if (data == node->data) {
        return true;
    }
    if (data < node->data) {
        return search(node->left, data);
    }
    return search(node->right, data);
}

// اختبار بنية بيانات شجرة البحث الثنائية
void runTests() {
    struct Node* root = NULL;

    // Test insert and search
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    assert(search(root, 50) == true);
    assert(search(root, 30) == true);
    assert(search(root, 70) == true);
    assert(search(root, 20) == true);
    assert(search(root, 40) == true);
    assert(search(root, 60) == true);
    assert(search(root, 80) == true);
    assert(search(root, 100) == false);

    // Test delete leaf node
    root = deleteNode(root, 20);
    assert(search(root, 20) == false);

    // Test delete node with one child
    root = deleteNode(root, 30);
    assert(search(root, 30) == false);
    assert(search(root, 40) == true);

    // Test delete node with two children
    root = deleteNode(root, 70);
    assert(search(root, 70) == false);
    assert(search(root, 60) == true);
    assert(search(root, 80) == true);

    // Test delete root node
    root = deleteNode(root, 50);
    assert(search(root, 50) == false);
    assert(search(root, 40) == true);

    printf("All BST test cases passed!\n");
}

int main() {
    runTests();
    return 0;
}
