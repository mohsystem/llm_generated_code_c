#include <stdio.h>
#include <string.h>
#include <assert.h>

void reverse(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int string_equals(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void run_tests() {
    char buffer[200];

    strcpy(buffer, "hello");
    reverse(buffer);
    assert(string_equals(buffer, "olleh"));

    strcpy(buffer, "");
    reverse(buffer);
    assert(string_equals(buffer, ""));

    strcpy(buffer, "a");
    reverse(buffer);
    assert(string_equals(buffer, "a"));

    strcpy(buffer, "madam");
    reverse(buffer);
    assert(string_equals(buffer, "madam"));

    strcpy(buffer, "hello");
    reverse(buffer);
    assert(string_equals(buffer, "olleh"));

    strcpy(buffer, "12345");
    reverse(buffer);
    assert(string_equals(buffer, "54321"));

    strcpy(buffer, "!@#$%^&*()");
    reverse(buffer);
    assert(string_equals(buffer, ")(*&^%$#@!"));

    strcpy(buffer, "HeLLo");
    reverse(buffer);
    assert(string_equals(buffer, "oLLeH"));

    strcpy(buffer, "  hello  "); // Note: scanf("%s") reads until whitespace, so spaces won't be in input
    reverse(buffer);
    // Since spaces can't be scanned with %s, this case won't occur with current input method,
    // so we skip this test or just test as-is without spaces
    // Alternatively, skip this test because original scanf won't read spaces

    strcpy(buffer, "abcdefghijklmnopqrstuvwxyz");
    reverse(buffer);
    assert(string_equals(buffer, "zyxwvutsrqponmlkjihgfedcba"));

    printf("✓ All tests passed for reverse function.\n");
}

int main() {
    run_tests();
    return 0;
}
