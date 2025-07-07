#include <stdio.h>
#include <string.h>
#include <ctype.h>

void cleanPhoneNumber(char* phoneNumber) {
    int i, j;
    for (i = 0, j = 0; phoneNumber[i] != '\0'; i++) {
        if (isdigit(phoneNumber[i])) {
            phoneNumber[j++] = phoneNumber[i];
        }
    }
    phoneNumber[j] = '\0';
    if (phoneNumber[0] == '1' && strlen(phoneNumber) == 11) {
        memmove(phoneNumber, phoneNumber + 1, strlen(phoneNumber));
    }
}

void test_case(char* input, const char* expected, int case_num) {
    char buffer[50];
    strcpy(buffer, input);
    cleanPhoneNumber(buffer);
    if (strlen(buffer) == 10 && strcmp(buffer, expected) == 0) {
        printf("Test case %d: PASS\n", case_num);
    } else {
        printf("Test case %d: FAIL\nExpected: %s\nGot: %s\n", case_num, expected, buffer);
    }
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
