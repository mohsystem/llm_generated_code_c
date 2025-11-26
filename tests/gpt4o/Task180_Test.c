#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- إعلان الدوال للمساعدة ---


typedef struct TreeNode {
    int val;
    struct TreeNode** children;
    int childrenCount;
} TreeNode;

TreeNode* newTreeNode(int val) {
    TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    node->val = val;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}

TreeNode* reorientTree(TreeNode* node, TreeNode* root) {
    if (root == NULL) root = newTreeNode(node->val);
    root->children = (TreeNode**) malloc(node->childrenCount * sizeof(TreeNode*));
    root->childrenCount = node->childrenCount;
    for (int i = 0; i < node->childrenCount; i++) {
        root->children[i] = reorientTree(node->children[i], NULL);
    }
    return root;
}

void printTree(TreeNode* node, int level) {
    for (int i = 0; i < level * 2; i++) printf(" ");
    printf("%d\n", node->val);
    for (int i = 0; i < node->childrenCount; i++) {
        printTree(node->children[i], level + 1);
    }
}


// لبناء شجرة الاختبار الأصلية
TreeNode* buildExampleTree() {
    TreeNode* n[10];
    for (int i = 0; i < 10; i++) n[i] = newTreeNode(i);

    n[0]->childrenCount = 3;
    n[0]->children = (TreeNode**) malloc(3 * sizeof(TreeNode*));
    n[0]->children[0] = n[1];
    n[0]->children[1] = n[2];
    n[0]->children[2] = n[3];

    n[1]->childrenCount = 2;
    n[1]->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n[1]->children[0] = n[4];
    n[1]->children[1] = n[5];

    n[2]->childrenCount = 2;
    n[2]->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n[2]->children[0] = n[6];
    n[2]->children[1] = n[7];

    n[3]->childrenCount = 2;
    n[3]->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n[3]->children[0] = n[8];
    n[3]->children[1] = n[9];

    return n[0];
}

// بناء الشجرة المتوقعة عند إعادة التوجيه للجذر 6 (مبنية يدوي)
TreeNode* buildExpectedTreeFor6() {
    TreeNode* n0 = newTreeNode(6);
    TreeNode* n1 = newTreeNode(7);
    TreeNode* n2 = newTreeNode(2);
    TreeNode* n3 = newTreeNode(0);
    TreeNode* n4 = newTreeNode(1);
    TreeNode* n5 = newTreeNode(5);
    TreeNode* n6 = newTreeNode(4);
    TreeNode* n7 = newTreeNode(3);
    TreeNode* n8 = newTreeNode(8);
    TreeNode* n9 = newTreeNode(9);

    n0->childrenCount = 2;
    n0->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n0->children[0] = n1;
    n0->children[1] = n2;

    n1->childrenCount = 0;
    n1->children = NULL;

    n2->childrenCount = 2;
    n2->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n2->children[0] = n1;
    n2->children[1] = n3;

    n3->childrenCount = 2;
    n3->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n3->children[0] = n4;
    n3->children[1] = n7;

    n4->childrenCount = 2;
    n4->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n4->children[0] = n6;
    n4->children[1] = n5;

    n5->childrenCount = 0;
    n5->children = NULL;

    n6->childrenCount = 0;
    n6->children = NULL;

    n7->childrenCount = 2;
    n7->children = (TreeNode**) malloc(2 * sizeof(TreeNode*));
    n7->children[0] = n8;
    n7->children[1] = n9;

    n8->childrenCount = 0;
    n8->children = NULL;

    n9->childrenCount = 0;
    n9->children = NULL;

    return n0;
}

// مقارنة شجرتين للتحقق من التطابق التام (القيمة والبنية)
int treesEqual(TreeNode* a, TreeNode* b) {
    if (a == NULL && b == NULL) return 1;
    if (a == NULL || b == NULL) return 0;
    if (a->val != b->val) return 0;
    if (a->childrenCount != b->childrenCount) return 0;
    for (int i = 0; i < a->childrenCount; i++) {
        if (!treesEqual(a->children[i], b->children[i])) return 0;
    }
    return 1;
}

// تحرير ذاكرة الشجرة (الترتيب مهم)
void freeTree(TreeNode* node) {
    if (!node) return;
    for (int i = 0; i < node->childrenCount; i++) {
        freeTree(node->children[i]);
    }
    free(node->children);
    free(node);
}

// إيجاد عقدة بالقيمة val (مساعدة)
TreeNode* findNode(TreeNode* root, int val) {
    if (!root) return NULL;
    if (root->val == val) return root;
    for (int i = 0; i < root->childrenCount; i++) {
        TreeNode* found = findNode(root->children[i], val);
        if (found) return found;
    }
    return NULL;
}

// دالة اختبار صارمة للحالة
void test_reorient() {
    TreeNode* original = buildExampleTree();
    TreeNode* newRoot = reorientTree(findNode(original, 6), NULL);
    TreeNode* expected = buildExpectedTreeFor6();

    if (treesEqual(newRoot, expected)) {
        printf("Test reorient: PASS\n");
    } else {
        printf("Test reorient: FAIL\n");
    }

    freeTree(original);
    freeTree(newRoot);
    freeTree(expected);
}

// استدعاء الاختبار في main
int main() {
    test_reorient();

    return 0;
}
