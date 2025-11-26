#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "uthash.h"  // Include the uthash library

#define MAX_LENGTH 2000
#define MAX_SUBSTRINGS 10000

char substrings[MAX_SUBSTRINGS][MAX_LENGTH];
int substring_count = 0;

bool is_in_set(const char* str) {
    for (int i = 0; i < substring_count; i++) {
        if (strcmp(substrings[i], str) == 0) {
            return true;
        }
    }
    return false;
}

void add_to_set(const char* str) {
    if (!is_in_set(str) && substring_count < MAX_SUBSTRINGS) {
        strcpy(substrings[substring_count], str);
        substring_count++;
    }
}

int distinctEchoSubstrings(char* text) {
    int n = strlen(text);
    substring_count = 0;
    
    for (int i = 0; i < n; i++) {
        for (int length = 1; length <= (n - i) / 2; length++) {
            if (strncmp(text + i, text + i + length, length) == 0) {
                char substr[MAX_LENGTH];
                strncpy(substr, text + i, 2 * length);
                substr[2 * length] = '\0';
                add_to_set(substr);
            }
        }
    }
    
    return substring_count;
}

int main() {
    printf("%d\n", distinctEchoSubstrings("abcabcabc"));  // Output: 3
    printf("%d\n", distinctEchoSubstrings("leetcodeleetcode"));  // Output: 2
    return 0;
}