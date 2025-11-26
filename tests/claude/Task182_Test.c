#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PHONE_LENGTH 20

char* clean_phone_number(const char* phone) {
    char* digits = malloc(MAX_PHONE_LENGTH * sizeof(char));
    if (!digits) return NULL;
    int j = 0;

    for (int i = 0; phone[i] != '\0'; i++) {
        if (isdigit(phone[i])) {
            digits[j++] = phone[i];
        }
    }
    digits[j] = '\0';

    int len = strlen(digits);
    if (len == 11 && digits[0] == '1') {
        memmove(digits, digits + 1, len);  // including null terminator
    } else if (len != 10) {
        free(digits);
        return NULL;
    }

    return digits;
}

void test_case(const char* input, const char* expected, int case_num) {
    char* result = clean_phone_number(input);
    if (result == NULL) {
        printf("Test case %d: FAIL (returned NULL)\n", case_num);
        return;
    }
    if (strcmp(result, expected) == 0) {
        printf("Test case %d: PASS\n", case_num);
    } else {
        printf("Test case %d: FAIL (got '%s', expected '%s')\n", case_num, result, expected);
    }
    free(result);
}

int main() {
    test_case("+1 (613)-995-0253", "6139950253", 1);
    test_case("613-995-0253", "6139950253", 2);
    test_case("1 613 995 0253", "6139950253", 3);
    test_case("613.995.0253", "6139950253", 4);
    test_case("6139950253", "6139950253", 5);
    test_case("  +1  613  995  0253  ", "6139950253", 6);
    test_case("(613) 995-0253", "6139950253", 7);
    test_case("1-613-995-0253", "6139950253", 8);
    test_case("1 613.995 0253", "6139950253", 9);
    test_case("613 995 0253", "6139950253", 10);

    return 0;
}
