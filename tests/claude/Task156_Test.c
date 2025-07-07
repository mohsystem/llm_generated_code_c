#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Traversal functions fill the array and update index
void preorderTraversal(struct TreeNode* root, int* arr, int* index) {
    if (root == NULL) return;
    arr[(*index)++] = root->val;
    preorderTraversal(root->left, arr, index);
    preorderTraversal(root->right, arr, index);
}

void inorderTraversal(struct TreeNode* root, int* arr, int* index) {
    if (root == NULL) return;
    inorderTraversal(root->left, arr, index);
    arr[(*index)++] = root->val;
    inorderTraversal(root->right, arr, index);
}

void postorderTraversal(struct TreeNode* root, int* arr, int* index) {
    if (root == NULL) return;
    postorderTraversal(root->left, arr, index);
    postorderTraversal(root->right, arr, index);
    arr[(*index)++] = root->val;
}

// Function to compare two arrays
int arraysEqual(int* a, int* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

// Test function
void runTests() {
    struct TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);

    int preorderExpected[] = {1, 2, 4, 5, 3};
    int inorderExpected[] = {4, 2, 5, 1, 3};
    int postorderExpected[] = {4, 5, 2, 3, 1};

    int result[100];
    int index;

    // Preorder test
    index = 0;
    preorderTraversal(root, result, &index);
    if (arraysEqual(result, preorderExpected, index))
        printf("Preorder test passed!\n");
    else
        printf("Preorder test FAILED!\n");

    // Inorder test
    index = 0;
    inorderTraversal(root, result, &index);
    if (arraysEqual(result, inorderExpected, index))
        printf("Inorder test passed!\n");
    else
        printf("Inorder test FAILED!\n");

    // Postorder test
    index = 0;
    postorderTraversal(root, result, &index);
    if (arraysEqual(result, postorderExpected, index))
        printf("Postorder test passed!\n");
    else
        printf("Postorder test FAILED!\n");
}

int main() {
    runTests();
    return 0;
}
