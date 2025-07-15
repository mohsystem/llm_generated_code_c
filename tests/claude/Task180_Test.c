#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 10

typedef struct Node {
    int data;
    struct Node* children[MAX_CHILDREN];
    int childCount;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->childCount = 0;
    for(int i=0; i<MAX_CHILDREN; i++) newNode->children[i] = NULL;
    return newNode;
}

Node* findNode(Node* node, int data) {
    if (!node) return NULL;
    if (node->data == data) {
        return node;
    }
    for (int i = 0; i < node->childCount; i++) {
        Node* result = findNode(node->children[i], data);
        if (result) {
            return result;
        }
    }
    return NULL;
}

void removeChild(Node* parent, Node* child) {
    int index = -1;
    for (int i = 0; i < parent->childCount; i++) {
        if (parent->children[i] == child) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index; i < parent->childCount - 1; i++) {
            parent->children[i] = parent->children[i + 1];
        }
        parent->children[parent->childCount - 1] = NULL;
        parent->childCount--;
    }
}

void addChild(Node* parent, Node* child) {
    if (parent->childCount < MAX_CHILDREN) {
        parent->children[parent->childCount++] = child;
    }
}

void reparentHelper(Node* node, Node* parent) {
    if (parent) {
        // That will cause 0% pass cases, it considered the parent as a child ...
        removeChild(node, parent);
        addChild(node, parent);
    }
    for (int i = 0; i < node->childCount; i++) {
        Node* child = node->children[i];
        if (child != parent) {
            reparentHelper(child, node);
        }
    }
}

Node* reparent(Node* root, int newRootData) {
    Node* newRoot = findNode(root, newRootData);
    if (!newRoot) {
        return root;
    }
    reparentHelper(newRoot, NULL);
    return newRoot;
}

void printTree(Node* node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%d\n", node->data);
    for (int i = 0; i < node->childCount; i++) {
        printTree(node->children[i], level + 1);
    }
}

// دالة مساعدة للتحقق إذا الأب node يحتوي الابن child
int hasChild(Node* parent, Node* child) {
    for (int i = 0; i < parent->childCount; i++) {
        if (parent->children[i] == child)
            return 1;
    }
    return 0;
}

// تحرير الذاكرة (اختياري لكن جيد)
void freeTree(Node* node) {
    if (!node) return;
    for (int i = 0; i < node->childCount; i++) {
        freeTree(node->children[i]);
    }
    free(node);
}

// ======== Unit Tests ========

int test_reparent_basic() {
    Node* root = createNode(0);
    addChild(root, createNode(1));
    addChild(root, createNode(2));
    addChild(root, createNode(3));

    addChild(root->children[0], createNode(4));
    addChild(root->children[0], createNode(5));

    addChild(root->children[1], createNode(6));
    addChild(root->children[1], createNode(7));

    addChild(root->children[2], createNode(8));
    addChild(root->children[2], createNode(9));

    Node* newRoot = reparent(root, 6);

    // بعد إعادة التوجيه، 6 يجب أن يكون الجذر
    if (newRoot->data != 6) return 0;
    // الجذر الجديد يجب أن يحتوي على 2 و 7
    if (!hasChild(newRoot, findNode(newRoot, 2))) return 0;
    if (!hasChild(newRoot, findNode(newRoot, 7))) return 0;
    // 2 يجب أن يحتوي على 0 (الأب القديم) و 7
    Node* node2 = findNode(newRoot, 2);
    Node* node0 = findNode(newRoot, 0);
    if (!hasChild(node2, node0)) return 0;

    freeTree(newRoot);
    return 1;
}

int test_reparent_root_node() {
    Node* root = createNode(10);
    addChild(root, createNode(20));
    addChild(root, createNode(30));

    Node* newRoot = reparent(root, 10);  // إعادة التوجيه على نفس الجذر

    if (newRoot != root) return 0;
    if (newRoot->childCount != 2) return 0;

    freeTree(newRoot);
    return 1;
}

int test_reparent_nonexistent_node() {
    Node* root = createNode(100);
    addChild(root, createNode(200));

    Node* newRoot = reparent(root, 999);  // لا يوجد هذا العنصر

    if (newRoot != root) return 0;
    if (newRoot->childCount != 1) return 0;

    freeTree(newRoot);
    return 1;
}

void run_tests() {
    printf("Running unit tests...\n");

    printf("Test basic reparent: %s\n", test_reparent_basic() ? "PASS" : "FAIL");
    printf("Test reparent root node: %s\n", test_reparent_root_node() ? "PASS" : "FAIL");
    printf("Test reparent nonexistent node: %s\n", test_reparent_nonexistent_node() ? "PASS" : "FAIL");
}

int main() {
    Node* root = createNode(0);
    addChild(root, createNode(1));
    addChild(root, createNode(2));
    addChild(root, createNode(3));

    addChild(root->children[0], createNode(4));
    addChild(root->children[0], createNode(5));

    addChild(root->children[1], createNode(6));
    addChild(root->children[1], createNode(7));

    addChild(root->children[2], createNode(8));
    addChild(root->children[2], createNode(9));

    printf("Original tree:\n");
    printTree(root, 0);

    Node* newRoot = reparent(root, 6);
    printf("\nReparented tree with 6 as root:\n");
    printTree(newRoot, 0);

    freeTree(newRoot);

    // شغل التستات
    run_tests();

    return 0;
}
