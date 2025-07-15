#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node** children;
    int children_count;
} Node;

Node* newNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

void addChild(Node* parent, Node* child) {
    parent->children_count++;
    parent->children = (Node**)realloc(parent->children, parent->children_count * sizeof(Node*));
    parent->children[parent->children_count - 1] = child;
}

void reparent(Node* node, Node* parent) {
    // Remove parent from node's children, if exists
    for (int i = 0; i < node->children_count; i++) {
        if (node->children[i] == parent) {
            for (int j = i; j < node->children_count - 1; j++) {
                node->children[j] = node->children[j + 1];
            }
            node->children_count--;
            if (node->children_count > 0)
                node->children = (Node**)realloc(node->children, node->children_count * sizeof(Node*));
            else {
                free(node->children);
                node->children = NULL;
            }
            break;
        }
    }
    // Recursively reparent children
    for (int i = 0; i < node->children_count; i++) {
        reparent(node->children[i], node);
    }
    // Add parent as child of node if parent != NULL
    if (parent != NULL) {
        addChild(node, parent);
    }
}

void printTree(Node* node, int level) {
    for (int i = 0; i < level; i++) {
        printf(" ");
    }
    printf("%d\n", node->data);
    for (int i = 0; i < node->children_count; i++) {
        printTree(node->children[i], level + 1);
    }
}

// Helper: find node by data
Node* findNode(Node* root, int data) {
    if (root->data == data) return root;
    for (int i = 0; i < root->children_count; i++) {
        Node* found = findNode(root->children[i], data);
        if (found) return found;
    }
    return NULL;
}

// Helper: check if parent has child pointer
int hasChild(Node* parent, Node* child) {
    for (int i = 0; i < parent->children_count; i++) {
        if (parent->children[i] == child)
            return 1;
    }
    return 0;
}

// Helper: free entire tree recursively
void freeTree(Node* node) {
    for (int i = 0; i < node->children_count; i++) {
        freeTree(node->children[i]);
    }
    free(node->children);
    free(node);
}

// === Unit Tests ===

int test_basic_reparent() {
    Node* root = newNode(0);
    Node* n1 = newNode(1);
    Node* n2 = newNode(2);
    Node* n3 = newNode(3);
    addChild(root, n1);
    addChild(root, n2);
    addChild(root, n3);

    addChild(n1, newNode(4));
    addChild(n1, newNode(5));

    Node* n6 = newNode(6);
    addChild(n2, n6);
    addChild(n2, newNode(7));

    addChild(n3, newNode(8));
    addChild(n3, newNode(9));

    reparent(n6, n2); // Reparent on node 6

    if (n6->data != 6) return 0;
    if (!hasChild(n6, n2)) return 0;
    if (!hasChild(n2, root)) return 0;
    if (hasChild(n2, n6)) return 0;  // n6 should no longer be child of n2

    freeTree(root);
    return 1;
}

int test_reparent_root() {
    Node* root = newNode(10);
    addChild(root, newNode(20));
    addChild(root, newNode(30));

    reparent(root, NULL);

    if (root->children_count != 2) return 0;

    freeTree(root);
    return 1;
}

int test_reparent_nonexistent() {
    Node* root = newNode(100);
    addChild(root, newNode(200));

    // Try reparenting with NULL parent on node not connected, just root here
    reparent(root, NULL);

    if (root->children_count != 1) return 0;

    freeTree(root);
    return 1;
}

void run_tests() {
    printf("Running tests...\n");
    printf("Test basic reparent: %s\n", test_basic_reparent() ? "PASS" : "FAIL");
    printf("Test reparent root: %s\n", test_reparent_root() ? "PASS" : "FAIL");
    printf("Test reparent nonexistent: %s\n", test_reparent_nonexistent() ? "PASS" : "FAIL");
}

int main() {
    run_tests();
    return 0;
}
