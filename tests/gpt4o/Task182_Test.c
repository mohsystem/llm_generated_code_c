#include <stdio.h>
#include <string.h>
#include <ctype.h>

void cleanNumber(char* input, char* output) {
    int j = 0;
    for (int i = 0; i < strlen(input); i++) {
        if (isdigit(input[i])) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';

    if (output[0] == '1' && strlen(output) == 11) {
        for (int i = 1; i < 11; i++) {
            output[i-1] = output[i];
        }
        output[10] = '\0';
    }
}

void test_case(char* input, const char* expected, int case_num) {
    char output[20];
    cleanNumber(input, output);
    if (strlen(output) == 10 && strcmp(output, expected) == 0) {
        printf("Test case %d: PASS\n", case_num);
    } else {
        printf("Test case %d: FAIL\nExpected: %s\nGot: %s\n", case_num, expected, output);
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
