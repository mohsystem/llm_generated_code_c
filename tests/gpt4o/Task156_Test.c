#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;
    struct Node* left, * right;
};

struct Node* newNode(int item) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// جمع قيم الـ preorder
void collectPreorder(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    arr[(*index)++] = node->key;
    collectPreorder(node->left, arr, index);
    collectPreorder(node->right, arr, index);
}

// جمع قيم الـ inorder
void collectInorder(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    collectInorder(node->left, arr, index);
    arr[(*index)++] = node->key;
    collectInorder(node->right, arr, index);
}

// جمع قيم الـ postorder
void collectPostorder(struct Node* node, int* arr, int* index) {
    if (node == NULL)
        return;
    collectPostorder(node->left, arr, index);
    collectPostorder(node->right, arr, index);
    arr[(*index)++] = node->key;
}

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
    int inorderExpected[] =  {4, 2, 5, 1, 3};
    int postorderExpected[] ={4, 5, 2, 3, 1};

    int result[100];
    int index;

    // اختبار Preorder
    index = 0;
    collectPreorder(root, result, &index);
    printf("Preorder traversal test: %s\n", arraysEqual(result, preorderExpected, index) ? "PASSED" : "FAILED");

    // اختبار Inorder
    index = 0;
    collectInorder(root, result, &index);
    printf("Inorder traversal test: %s\n", arraysEqual(result, inorderExpected, index) ? "PASSED" : "FAILED");

    // اختبار Postorder
    index = 0;
    collectPostorder(root, result, &index);
    printf("Postorder traversal test: %s\n", arraysEqual(result, postorderExpected, index) ? "PASSED" : "FAILED");
}

int main() {
    runTests();
    return 0;
}
