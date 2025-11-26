#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
    char* str;
    struct Node* next;
} Node;

// Create a new node with a given string
Node* createNode(char* str) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->str = strdup(str);  // Duplicate the string to avoid modification
    node->next = NULL;
    return node;
}

// Check if a string exists in the linked list
int find(Node* head, char* str) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->str, str) == 0) {
            return 1;  // Found the string
        }
        current = current->next;
    }
    return 0;  // String not found
}

// Insert a string into the linked list
void insert(Node** head, char* str) {
    Node* new_node = createNode(str);
    new_node->next = *head;
    *head = new_node;
}

// Function to count distinct "a + a" substrings
int distinctAASubstrings(char* text) {
    int n = strlen(text);
    Node* distinctSubstrings = NULL;  // Linked list to store distinct substrings
    int count = 0;

    // Iterate through all substrings
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; j += 2) {  // Ensure we check only even-length substrings
            int len = (j + 1 - i) / 2;
            char a[len + 1];
            char b[len + 1];
            strncpy(a, text + i, len);
            a[len] = '\0';
            strncpy(b, text + i + len, len);
            b[len] = '\0';

            // Check if substring can be split into two identical parts
            if (strcmp(a, b) == 0) {
                char* substr = (char*)malloc((j - i + 2) * sizeof(char));
                strncpy(substr, text + i, j - i + 1);
                substr[j - i + 1] = '\0';

                // If substring is distinct, insert it into the list
                if (!find(distinctSubstrings, substr)) {
                    insert(&distinctSubstrings, substr);
                    count++;
                }
                free(substr);
            }
        }
    }

    // Clean up the linked list
    Node* current = distinctSubstrings;
    while (current != NULL) {
        Node* next = current->next;
        free(current->str);
        free(current);
        current = next;
    }

    return count;
}

// Function to run a test case
void test_case(char* text, int expected) {
    int result = distinctAASubstrings(text);
    if (result == expected) {
        printf("Test Passed: %s -> %d\n", text, result);
    } else {
        printf("Test Failed: %s -> %d (Expected: %d)\n", text, result, expected);
    }
}

int main() {
    // Run all test cases
    test_case("abcabcabc", 3);  // Test Case 1
    test_case("leetcodeleetcode", 2);  // Test Case 2
    test_case("a", 0);  // Test Case 3
    test_case("aa", 1);  // Test Case 4
    test_case("aaa", 1);  // Test Case 5
    test_case("abab", 2);  // Test Case 6
    test_case("abcde", 0);  // Test Case 7
    test_case("abababab", 4);  // Test Case 8
    test_case("abcabcabcabc", 6);  // Test Case 9

    return 0;
}
