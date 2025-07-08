#include <stdio.h>
#include <string.h>
#include <assert.h>

void reverse_string(const char *input, char *output) {
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        output[i] = input[len - i - 1];
    }
    output[len] = '\0';
}

int string_equals(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

void run_tests() {
    char output[200];

    reverse_string("hello", output);
    assert(string_equals(output, "olleh"));

    reverse_string("", output);
    assert(string_equals(output, ""));

    reverse_string("a", output);
    assert(string_equals(output, "a"));

    reverse_string("madam", output);
    assert(string_equals(output, "madam"));

    reverse_string("hello world", output);
    assert(string_equals(output, "dlrow olleh"));

    reverse_string("12345", output);
    assert(string_equals(output, "54321"));

    reverse_string("!@#$%^&*()", output);
    assert(string_equals(output, ")(*&^%$#@!"));

    reverse_string("HeLLo", output);
    assert(string_equals(output, "oLLeH"));

    reverse_string("  hello  ", output);
    assert(string_equals(output, "  olleh  "));

    reverse_string("abcdefghijklmnopqrstuvwxyz", output);
    assert(string_equals(output, "zyxwvutsrqponmlkjihgfedcba"));

    printf("✓ All tests passed for reverse_string with output buffer.\n");
}

int main() {
    run_tests();
    return 0;
}
