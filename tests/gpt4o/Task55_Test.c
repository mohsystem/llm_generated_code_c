#include <stdio.h>
#include <regex.h>

int validateEmail(const char *email) {
    const char *pattern = "^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$";
    regex_t regex;
    int result;

    result = regcomp(&regex, pattern, REG_EXTENDED);
    if (result) {
        return 0;
    }

    result = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    if (!result) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    const char* test_cases[] = {
        "test.email@example.com",          // valid
        "user123@example.com",             // valid
        "user+name@example.co.uk",         // valid
        "invalidemail.com",                // invalid (missing @)
        "user@@example.com",               // invalid (double @)
        "user@.com",                      // invalid (missing domain part)
        "user@example..com",              // invalid (multiple dots)
        "@example.com",                   // invalid (missing username)
        "user@ex!ample.com",              // invalid (special char in domain)
        "user@example.c"                  // invalid (short TLD)
    };
    int expected[] = {1,1,1,0,0,0,0,0,0,0};
    int pass=0, fail=0;

    for(int i=0; i<10; i++) {
        int result = validateEmail(test_cases[i]);
        if(result == expected[i]) {
            printf("Test %d: PASS\n", i+1);
            pass++;
        } else {
            printf("Test %d: FAIL\n", i+1);
            fail++;
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
