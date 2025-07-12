#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// دالة التحقق من صحة البريد باستخدام نفس الـregex في كودك
int validateEmail(const char* email) {
    regex_t regex;
    int reti;
    // استخدم نفس التعبير المنتظم بالضبط من كودك (الهروب المزدوج مطلوب في C string)
    reti = regcomp(&regex, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    return reti == 0;
}

int main() {
    // قائمة الاختبارات من البايثون (نفس النصوص)
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

    // النتائج المتوقعة: 1 = valid, 0 = invalid
    int expected_results[] = {
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0
    };

    int pass = 0, fail = 0;

    for (int i = 0; i < 10; i++) {
        int result = validateEmail(test_cases[i]);
        if (result == expected_results[i]) {
            printf("Test %d: PASS\n", i + 1);
            pass++;
        } else {
            printf("Test %d: FAIL\n", i + 1);
            fail++;
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
