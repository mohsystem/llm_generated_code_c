#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "uthash.h"  // مكتبة uthash يجب أن تكون متاحة ومرفقة

#define MAX_LENGTH 2000

typedef struct {
    char* key;
    UT_hash_handle hh;
} HashEntry;

HashEntry* result = NULL;

void add_to_set(const char* str) {
    HashEntry* entry;
    HASH_FIND_STR(result, str, entry);
    if (entry == NULL) {
        entry = (HashEntry*)malloc(sizeof(HashEntry));
        entry->key = strdup(str);
        HASH_ADD_KEYPTR(hh, result, entry->key, strlen(entry->key), entry);
    }
}

int distinctEchoSubstrings(char* text) {
    int n = strlen(text);
    result = NULL;

    for (int i = 0; i < n; i++) {
        for (int length = 1; length <= (n - i) / 2; length++) {
            if (strncmp(text + i, text + i + length, length) == 0) {
                char* substr = (char*)malloc((2 * length + 1) * sizeof(char));
                strncpy(substr, text + i, 2 * length);
                substr[2 * length] = '\0';
                add_to_set(substr);
                free(substr);
            }
        }
    }

    int count = HASH_COUNT(result);

    HashEntry* current, *tmp;
    HASH_ITER(hh, result, current, tmp) {
        HASH_DEL(result, current);
        free(current->key);
        free(current);
    }

    return count;
}

void run_tests() {
    struct {
        char* input;
        int expected;
        char* description;
    } tests[] = {
        {"abcabcabc", 3, "Test Case 1"},
        {"leetcodeleetcode", 2, "Test Case 2"},
        {"a", 0, "Test Case 3"},
        {"aa", 1, "Test Case 4"},
        {"aaa", 1, "Test Case 5"},
        {"abab", 2, "Test Case 6"},
        {"abcde", 0, "Test Case 7"},
        // Note: The ambiguous test case 8: (4 or 2)
        // We will expect 4 as per your comment (adjust if you want 2)
        {"abababab", 4, "Test Case 8"},
        {"abcabcabcabc", 6, "Test Case 9"},
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < n; i++) {
        int output = distinctEchoSubstrings(tests[i].input);
        if (output == tests[i].expected) {
            printf("%s: PASS\n", tests[i].description);
        } else {
            printf("%s: FAIL (Expected %d, Got %d)\n", tests[i].description, tests[i].expected, output);
        }
    }
}

int main() {
    run_tests();
    return 0;
}
