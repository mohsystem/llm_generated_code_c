#include <stdio.h>
#include <regex.h>
#include <string.h>

int validate_regex(const char* pattern, const char* input) {
    regex_t regex;
    int reti;
    char msgbuf[100];

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    reti = regexec(&regex, input, 0, NULL, 0);
    regfree(&regex);
    if (!reti) {
        return 1;
    } else if (reti == REG_NOMATCH) {
        return 0;
    } else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        return 0;
    }
}

int validate_email(const char* input) {
    const char* email_pattern = "^[[:alnum:]\\\\.-]+@[[:alnum:]\\\\.-]+\\.[[:alpha:]]+$";
    return validate_regex(email_pattern, input);
}

int validate_phone(const char* input) {
    const char* phone_pattern = "^\\(?([0-9]{3})\\)?[-. ]?([0-9]{3})[-. ]?([0-9]{4})$";
    return validate_regex(phone_pattern, input);
}

int validate_password(const char* input) {
    const char* password_pattern = "^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])[a-zA-Z0-9]{8,}$";
    return validate_regex(password_pattern, input);
}

void run_test_case_email(const char* input, int expected) {
    int result = validate_email(input);
    printf("Email test '%s': %s\n", input, (result == expected) ? "PASS" : "FAIL");
}

void run_test_case_phone(const char* input, int expected) {
    int result = validate_phone(input);
    printf("Phone test '%s': %s\n", input, (result == expected) ? "PASS" : "FAIL");
}

void run_test_case_password(const char* input, int expected) {
    int result = validate_password(input);
    printf("Password test '%s': %s\n", input, (result == expected) ? "PASS" : "FAIL");
}

int main() {
    // Email tests
    run_test_case_email("test@example.com", 1);
    run_test_case_email("invalid-email", 0);
    run_test_case_email("user@domain.co.uk", 1);
    run_test_case_email("user@.com", 0);
    run_test_case_email("userdomain.com", 0);

    // Phone tests
    run_test_case_phone("(123) 456-7890", 1);
    run_test_case_phone("123-abc-7890", 0);
    run_test_case_phone("123-456-7890", 1);
    run_test_case_phone("123-456-78901", 0);

    // Password tests
    run_test_case_password("Password1", 1);
    run_test_case_password("password1", 0);
    run_test_case_password("Password", 0);
    run_test_case_password("Pass1", 0);

    return 0;
}
