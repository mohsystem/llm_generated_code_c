#include <stdio.h>
#include <regex.h>

int validateEmail(const char* email) {
    regex_t regex;
    int reti;
    const char* pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    if (!reti) {
        return 1;
    } else if (reti == REG_NOMATCH) {
        return 0;
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
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

    int expected_results[] = {1,1,1,0,0,0,0,0,0,0};
    int pass = 0, fail = 0;

    for (int i = 0; i < 10; i++) {
        int res = validateEmail(test_cases[i]);
        if (res == expected_results[i]) {
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
