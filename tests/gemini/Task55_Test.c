#include <stdio.h>
#include <string.h>
#include <regex.h>

int isValidEmail(const char *email) {
    regex_t regex;
    int result;

    // Compile the regular expression
    result = regcomp(&regex, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", REG_EXTENDED);
    if (result) {
        return 0; // Failed to compile
    }

    // Match the email against the regex
    result = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    return result == 0; // 0 means match, non-zero means no match
}

int main() {
    const char* test_cases[] = {
        "test.email@example.com",        // valid
        "user123@example.com",           // valid
        "user+name@example.co.uk",       // valid
        "invalidemail.com",              // invalid (missing @)
        "user@@example.com",             // invalid (double @)
        "user@.com",                    // invalid (missing domain part)
        "user@example..com",            // invalid (multiple dots)
        "@example.com",                 // invalid (missing username)
        "user@ex!ample.com",            // invalid (special char in domain)
        "user@example.c"                // invalid (short TLD)
      };
    int expected[] = {1,1,1,0,0,0,0,0,0,0};
    int pass=0, fail=0;

    for(int i=0; i<10; i++) {
        int result = isValidEmail(test_cases[i]);
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
