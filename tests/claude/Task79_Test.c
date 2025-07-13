#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 1000

void shuffle(char *array, int n, unsigned int seed) {
    srand(seed);
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void encrypt_message(const char *message, char *encrypted, int key) {
    char cipher[26];
    for (int i = 0; i < 26; i++) {
        cipher[i] = 'A' + i;
    }

    shuffle(cipher, 26, key);

    int i = 0;
    while (message[i] != '\0') {
        if (isalpha(message[i])) {
            int index = toupper(message[i]) - 'A';
            encrypted[i] = cipher[index];
        } else {
            encrypted[i] = message[i];
        }
        i++;
    }
    encrypted[i] = '\0';
}

// مقارنة نصوص (case sensitive)
int strings_equal(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void run_test_cases() {
    struct TestCase {
        const char* input;
        int key;
        const char* expected;
        const char* description;
    } tests[] = {
        {"HELLO", 3, "KHOOR", "basic encryption"},
        {"HELLO WORLD", 5, "QNUXZ DMJFZ", "encryption with spaces"},
        {"HELLO, WORLD!", 2, "RDSSV, UFSVF!", "encryption with special chars"},
        {"", 10, "", "empty message"},
        {"HELLO123", 4, "GDKKN123", "encryption with numbers"},
        {"PYTHON", 6, "VEDTUS", "same key produces same output"},
        {"PYTHON", 8, "XAFVQP", "different key produces different output"},
        {"Python", 7, "DKSOQD", "case insensitive encryption"},
        {"TEST", 9, "UQEB", "key affects shuffling"},
        {"test", 11, "OJPP", "same key and message different cases"}
    };

    char encrypted[MAX_LENGTH];

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        encrypt_message(tests[i].input, encrypted, tests[i].key);
        int pass = strings_equal(encrypted, tests[i].expected);
        printf("Test case %d (%s): %s\n", i+1, tests[i].description, pass ? "PASS" : "FAIL");
        if (!pass) {
            printf("  Input: \"%s\" with key %d\n", tests[i].input, tests[i].key);
            printf("  Expected: %s\n", tests[i].expected);
            printf("  Got: %s\n", encrypted);
        }
    }
}

int main() {
    run_test_cases();
    return 0;
}
