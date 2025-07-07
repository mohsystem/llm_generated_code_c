#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Fill array with preorder traversal
void collectPreorder(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    arr[(*index)++] = node->data;
    collectPreorder(node->left, arr, index);
    collectPreorder(node->right, arr, index);
}

// Fill array with inorder traversal
void collectInorder(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    collectInorder(node->left, arr, index);
    arr[(*index)++] = node->data;
    collectInorder(node->right, arr, index);
}

// Fill array with postorder traversal
void collectPostorder(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    collectPostorder(node->left, arr, index);
    collectPostorder(node->right, arr, index);
    arr[(*index)++] = node->data;
}

// Compare two arrays
int arraysEqual(int* a, int* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

void runTests() {
    struct Node* root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);

    int preorderExpected[] = {1, 2, 4, 5, 3};
    int inorderExpected[] = {4, 2, 5, 1, 3};
    int postorderExpected[] = {4, 5, 2, 3, 1};

    int result[100];
    int index;

    // Test preorder
    index = 0;
    collectPreorder(root, result, &index);
    if (arraysEqual(result, preorderExpected, index))
        printf("Preorder test passed!\n");
    else
        printf("Preorder test FAILED!\n");

    // Test inorder
    index = 0;
    collectInorder(root, result, &index);
    if (arraysEqual(result, inorderExpected, index))
        printf("Inorder test passed!\n");
    else
        printf("Inorder test FAILED!\n");

    // Test postorder
    index = 0;
    collectPostorder(root, result, &index);
    if (arraysEqual(result, postorderExpected, index))
        printf("Postorder test passed!\n");
    else
        printf("Postorder test FAILED!\n");
}

int main() {
    runTests();
    return 0;
}
