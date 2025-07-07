#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

void preorder_traversal(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    arr[(*index)++] = node->data;
    preorder_traversal(node->left, arr, index);
    preorder_traversal(node->right, arr, index);
}

void inorder_traversal(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    inorder_traversal(node->left, arr, index);
    arr[(*index)++] = node->data;
    inorder_traversal(node->right, arr, index);
}

void postorder_traversal(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    postorder_traversal(node->left, arr, index);
    postorder_traversal(node->right, arr, index);
    arr[(*index)++] = node->data;
}

int arraysEqual(int* a, int* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void runTests(struct Node* root) {
    int preorderExpected[] = {1, 2, 4, 5, 3};
    int inorderExpected[]  = {4, 2, 5, 1, 3};
    int postorderExpected[]= {4, 5, 2, 3, 1};
    int arr[100], idx;

    // Test preorder
    idx = 0;
    preorder_traversal(root, arr, &idx);
    printf("Preorder test: %s\n", arraysEqual(arr, preorderExpected, idx) ? "PASSED" : "FAILED");

    // Test inorder
    idx = 0;
    inorder_traversal(root, arr, &idx);
    printf("Inorder test: %s\n", arraysEqual(arr, inorderExpected, idx) ? "PASSED" : "FAILED");

    // Test postorder
    idx = 0;
    postorder_traversal(root, arr, &idx);
    printf("Postorder test: %s\n", arraysEqual(arr, postorderExpected, idx) ? "PASSED" : "FAILED");
}

int main() {
    struct Node* root = (struct Node*)malloc(sizeof(struct Node));
    root->data = 1;
    root->left = (struct Node*)malloc(sizeof(struct Node));
    root->right = (struct Node*)malloc(sizeof(struct Node));
    root->left->data = 2;
    root->right->data = 3;
    root->left->left = (struct Node*)malloc(sizeof(struct Node));
    root->left->right = (struct Node*)malloc(sizeof(struct Node));
    root->left->left->data = 4;
    root->left->right->data = 5;
    root->left->left->left = root->left->left->right = NULL;
    root->left->right->left = root->left->right->right = NULL;
    root->right->left = root->right->right = NULL;

    runTests(root);

    return 0;
}
