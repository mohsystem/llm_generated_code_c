#include <stdio.h>
#include <string.h>
#include <ctype.h>

void clean_phone_number(char *phone_number) {
    char cleaned_number[20];  // كافية لتخزين 11 رقم + '\0'
    int j = 0;
    for (int i = 0; phone_number[i] != '\0'; i++) {
        if (isdigit(phone_number[i])) {
            cleaned_number[j++] = phone_number[i];
        }
    }
    cleaned_number[j] = '\0';

    if (strlen(cleaned_number) == 11 && cleaned_number[0] == '1') {
        // انقل الرقم بدون أول 1 إلى phone_number
        strcpy(phone_number, cleaned_number + 1);
    } else {
        strcpy(phone_number, cleaned_number);
    }
}

void test_case(char* input, const char* expected, int case_num) {
    char buffer[50];
    strcpy(buffer, input);
    clean_phone_number(buffer);
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
