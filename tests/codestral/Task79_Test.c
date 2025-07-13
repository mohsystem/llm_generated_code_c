#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_LENGTH 1000
void encrypt(const char* message, int key, char* cipher) {
    for (int i = 0; i < strlen(message); i++) {
        if (isalpha(message[i])) {
            char shifted = isupper(message[i]) ? 'A' : 'a';
            cipher[i] = (char) ((message[i] - shifted + key) % 26 + shifted);
        } else {
            cipher[i] = message[i];
        }
    }
    cipher[strlen(message)] = '\0';
}

// دالة لمقارنة النصوص
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
        {"HELLO WORLD", 5, "MJQQT BTWQI", "encryption with spaces"},
        {"HELLO, WORLD!", 2, "JGNNQ, YQTNF!", "encryption with special characters"},
        {"", 10, "", "empty message"},
        {"HELLO123", 4, "LIPPS123", "encryption with numbers"},
        {"PYTHON", 6, "VEDUTS", "same key produces same output"},
        {"PYTHON", 8, "XAFVQP", "different key produces different output"},
        {"Python", 7, "WFAAVU", "case insensitive encryption"},
        {"TEST", 9, "CDBC", "key affects shuffling"},
        {"test", 11, "efef", "same key and message different cases"},
    };

    char encrypted[MAX_LENGTH];
    int pass_count = 0;
    int fail_count = 0;

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        encrypt(tests[i].input, tests[i].key, encrypted);
        int pass = strings_equal(encrypted, tests[i].expected);
        printf("Test case %d (%s): %s\n", i+1, tests[i].description, pass ? "PASS" : "FAIL");
        if (!pass) {
            printf("  Input: \"%s\" with key %d\n", tests[i].input, tests[i].key);
            printf("  Expected: %s\n", tests[i].expected);
            printf("  Got: %s\n", encrypted);
        }
    }
}

#define MAX_LENGTH 1000

int main() {
    run_test_cases();
    return 0;
}
