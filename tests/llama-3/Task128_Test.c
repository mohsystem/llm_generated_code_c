#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// الكود الأصلي محفوظ كما هو:
char* generateToken(int length) {
    static char lettersAndDigits[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    static char token[11];
    for (int i = 0; i < length; i++) {
        token[i] = lettersAndDigits[rand() % (sizeof(lettersAndDigits) - 1)];
    }
    token[length] = '\0';
    return token;
}

// === Test framework ===
#define ASSERT_STR_LEN(str, len, testname) do { \
if ((int)strlen(str) == (len)) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_CHARS_IN_SET(str, allowed, testname) do { \
int valid = 1; \
for (int i = 0; str[i]; i++) { \
if (strchr(allowed, str[i]) == NULL) { \
valid = 0; \
break; \
} \
} \
if (valid) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

// === Test cases ===
void seedRandForTest() {
    srand(12345);
}

void test_token_length() {
    seedRandForTest();
    char* token = generateToken(10);
    ASSERT_STR_LEN(token, 10, "test_token_length");
}

void test_token_characters() {
    seedRandForTest();
    char* token = generateToken(10);
    const char* allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    ASSERT_CHARS_IN_SET(token, allowed, "test_token_characters");
}

int main() {
    test_token_length();
    test_token_characters();
    return 0;
}
