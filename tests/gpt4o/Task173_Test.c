#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    unsigned long long hash;
    int index;
    struct Node *next;
} Node;

Node* createNode(unsigned long long hash, int index) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->hash = hash;
    newNode->index = index;
    newNode->next = NULL;
    return newNode;
}

bool findNode(Node* head, const char *s, int length, int current, char *result) {
    while (head) {
        if (strncmp(s + head->index, s + current, length) == 0) {
            strncpy(result, s + head->index, length);
            result[length] = '\0';
            return true;
        }
        head = head->next;
    }
    return false;
}

void addNode(Node** hashTable, unsigned long long hash, int index, int size) {
    int position = hash % size;
    Node* newNode = createNode(hash, index);
    newNode->next = hashTable[position];
    hashTable[position] = newNode;
}

unsigned long long rollHash(const char *s, int length) {
    const unsigned long long p = 31, m = (1ULL << 61) - 1;
    unsigned long long hash_value = 0, p_pow = 1;
    for (int i = 0; i < length; ++i) {
        hash_value = (hash_value + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

void freeHashTable(Node** hashTable, int size) {
    for (int i = 0; i < size; i++) {
        Node* curr = hashTable[i];
        while (curr) {
            Node* next = curr->next;
            free(curr);
            curr = next;
        }
        hashTable[i] = NULL;
    }
}

void searchString(const char *s, int length, char *result) {
    const int size = 100003;
    Node* hashTable[size];
    for (int i = 0; i < size; ++i) hashTable[i] = NULL;

    const unsigned long long p = 31, m = (1ULL << 61) - 1;
    unsigned long long p_pow = 1, currentHash = rollHash(s, length);
    for (int i = 1; i < length; ++i) p_pow = (p_pow * p) % m;

    addNode(hashTable, currentHash, 0, size);

    int n = (int)strlen(s);
    for (int i = 1; i <= n - length; ++i) {
        currentHash = (currentHash + m - ((unsigned long long)(s[i - 1] - 'a' + 1) * p_pow) % m) % m;
        currentHash = (currentHash * p + (unsigned long long)(s[i + length - 1] - 'a' + 1)) % m;

        int position = currentHash % size;
        if (findNode(hashTable[position], s, length, i, result)) {
            freeHashTable(hashTable, size);
            return;
        }

        addNode(hashTable, currentHash, i, size);
    }
    result[0] = '\0';
    freeHashTable(hashTable, size);
}

void longestDupSubstring(const char *s, char *result) {
    int left = 1, right = (int)strlen(s) - 1, mid;
    char tempResult[10000];
    result[0] = '\0';

    while (left <= right) {
        mid = left + (right - left) / 2;
        searchString(s, mid, tempResult);

        if (tempResult[0] != '\0') {
            strcpy(result, tempResult);
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
}

// دالة مقارنة
int strEquals(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void runTest(const char* s, const char* expected, const char* testName) {
    char result[10000];
    longestDupSubstring(s, result);
    if (strEquals(result, expected)) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: \"%s\", Got: \"%s\"\n", testName, expected, result);
    }
}

int main() {
    runTest("abcabcabcd", "abcabc", "Test Case 1");
    runTest("abababcababab", "ababab", "Test Case 2");
    runTest("abacabadabacaba", "abacaba", "Test Case 3");
    runTest("abcdefgabcdefgabcdefg", "abcdefgabcdefg", "Test Case 4");
    runTest("aabbaabbaaccddcc", "aabbaa", "Test Case 5");
    runTest("abcdabcdeabcdabcdeabcd", "abcdabcdeabcd", "Test Case 6");
    runTest("abcdefghijklmnoabcdefghijklmno", "abcdefghijklmno", "Test Case 7");
    runTest("xyzxyzxyzxyzx", "xyzxyzxyzx", "Test Case 8");
    runTest("abcdefgh12345678abcdefgh", "abcdefgh", "Test Case 9");
    runTest("abcxabcyabczabcdabc", "abc", "Test Case 10");

    return 0;
}
