#include <stdio.h>
#include <string.h>
#include <assert.h>

void reverse_string(char* str) {
    int len = strlen(str);
    // إذا في \n بنزله عشان ما يعكسها مع الحروف
    if (len > 0 && str[len -1] == '\n') {
        str[len -1] = '\0';
        len--;
    }

    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i -1];
        str[len - i -1] = temp;
    }
}

int string_equals(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void run_tests() {
    char buffer[200];

    strcpy(buffer, "hello");
    reverse_string(buffer);
    assert(string_equals(buffer, "olleh"));

    strcpy(buffer, "");
    reverse_string(buffer);
    assert(string_equals(buffer, ""));

    strcpy(buffer, "a");
    reverse_string(buffer);
    assert(string_equals(buffer, "a"));

    strcpy(buffer, "madam");
    reverse_string(buffer);
    assert(string_equals(buffer, "madam"));

    strcpy(buffer, "hello world");
    reverse_string(buffer);
    assert(string_equals(buffer, "dlrow olleh"));

    strcpy(buffer, "12345");
    reverse_string(buffer);
    assert(string_equals(buffer, "54321"));

    strcpy(buffer, "!@#$%^&*()");
    reverse_string(buffer);
    assert(string_equals(buffer, ")(*&^%$#@!"));

    strcpy(buffer, "HeLLo");
    reverse_string(buffer);
    assert(string_equals(buffer, "oLLeH"));

    strcpy(buffer, "  hello  ");
    reverse_string(buffer);
    assert(string_equals(buffer, "  olleh  "));

    strcpy(buffer, "abcdefghijklmnopqrstuvwxyz");
    reverse_string(buffer);
    assert(string_equals(buffer, "zyxwvutsrqponmlkjihgfedcba"));

    printf("✓ All tests passed for reverse string.\n");
}

int main() {
    run_tests();
    return 0;
}
