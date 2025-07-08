#include <stdio.h>
#include <string.h>
#include <assert.h>

// دالة لعكس النص داخل المصفوفة نفسها
void reverseString(char* str) {
    int length = strlen(str);
    int i, j;
    char temp;
    for (i = 0, j = length - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

// دالة مساعدة للمقارنة بين نصين
int string_equals(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void run_tests() {
    char buffer[200];

    strcpy(buffer, "hello");
    reverseString(buffer);
    assert(string_equals(buffer, "olleh"));

    strcpy(buffer, "");
    reverseString(buffer);
    assert(string_equals(buffer, ""));

    strcpy(buffer, "a");
    reverseString(buffer);
    assert(string_equals(buffer, "a"));

    strcpy(buffer, "madam");
    reverseString(buffer);
    assert(string_equals(buffer, "madam"));

    strcpy(buffer, "hello world");
    reverseString(buffer);
    assert(string_equals(buffer, "dlrow olleh"));

    strcpy(buffer, "12345");
    reverseString(buffer);
    assert(string_equals(buffer, "54321"));

    strcpy(buffer, "!@#$%^&*()");
    reverseString(buffer);
    assert(string_equals(buffer, ")(*&^%$#@!"));

    strcpy(buffer, "HeLLo");
    reverseString(buffer);
    assert(string_equals(buffer, "oLLeH"));

    strcpy(buffer, "  hello  ");
    reverseString(buffer);
    assert(string_equals(buffer, "  olleh  "));

    strcpy(buffer, "abcdefghijklmnopqrstuvwxyz");
    reverseString(buffer);
    assert(string_equals(buffer, "zyxwvutsrqponmlkjihgfedcba"));

    printf("✓ All tests passed for reverse string.\n");
}

int main() {
    run_tests();
    return 0;
}
