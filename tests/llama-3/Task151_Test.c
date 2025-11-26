#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node {
    int val;
    struct Node* left;
    struct Node* right;
} Node;

Node* insert(Node* node, int val) {
    if (node == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->val = val;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (val < node->val) {
        node->left = insert(node->left, val);
    } else if (val > node->val) {
        node->right = insert(node->right, val);
    }
    return node;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node* deleteNode(Node* node, int val) {
    if (node == NULL) {
        return node;
    }
    if (val < node->val) {
        node->left = deleteNode(node->left, val);
    } else if (val > node->val) {
        node->right = deleteNode(node->right, val);
    } else {
        if (node->left == NULL) {
            Node* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Node* temp = node->left;
            free(node);
            return temp;
        }
        Node* temp = minValueNode(node->right);
        node->val = temp->val;
        node->right = deleteNode(node->right, temp->val);
    }
    return node;
}

Node* searchNode(Node* node, int val) {
    if (node == NULL || node->val == val) {
        return node;
    }
    if (node->val < val) {
        return searchNode(node->right, val);
    } else {
        return searchNode(node->left, val);
    }
}

void inorder(Node* node) {
    if (node != NULL) {
        inorder(node->left);
        printf("%d ", node->val);
        inorder(node->right);
    }
}

// Helper function to free the entire tree memory
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void runTests() {
    Node* root = NULL;

    // Insert nodes
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    // Test search existing nodes
    assert(searchNode(root, 50) != NULL);
    assert(searchNode(root, 30) != NULL);
    assert(searchNode(root, 20) != NULL);
    assert(searchNode(root, 40) != NULL);
    assert(searchNode(root, 70) != NULL);
    assert(searchNode(root, 60) != NULL);
    assert(searchNode(root, 80) != NULL);

    // Test search non-existing node
    assert(searchNode(root, 100) == NULL);

    // Delete leaf node
    root = deleteNode(root, 20);
    assert(searchNode(root, 20) == NULL);

    // Delete node with one child
    root = deleteNode(root, 30);
    assert(searchNode(root, 30) == NULL);
    assert(searchNode(root, 40) != NULL);

    // Delete node with two children
    root = deleteNode(root, 70);
    assert(searchNode(root, 70) == NULL);
    assert(searchNode(root, 60) != NULL);
    assert(searchNode(root, 80) != NULL);

    // Delete root node
    root = deleteNode(root, 50);
    assert(searchNode(root, 50) == NULL);

    // Final checks
    assert(searchNode(root, 40) != NULL);
    assert(searchNode(root, 60) != NULL);
    assert(searchNode(root, 80) != NULL);

    printf("All BST test cases passed!\n");

    freeTree(root);
}

int main() {
    runTests();
    return 0;
}
