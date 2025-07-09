#include <stdio.h>
#include <regex.h>
#include <string.h>

// دالة تحقق نمط regex
int match_regex(const char *pattern, const char *input) {
    regex_t regex;
    int ret;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) return -1;  // فشل تجميع التعبير

    ret = regexec(&regex, input, 0, NULL, 0);
    regfree(&regex);

    return (ret == 0) ? 1 : 0;
}

// دوال التحقق

const char* validate_email(const char* input) {
    const char* pattern = "^[A-Za-z0-9_.-]+@[A-Za-z0-9.-]+\\.[A-Za-z0-9_]+$";
    return match_regex(pattern, input) ? "Valid email" : "Invalid email";
}

const char* validate_phone(const char* input) {
    const char* pattern = "^\\(?[0-9]{3}\\)?[-. ]?[0-9]{3}[-. ]?[0-9]{4}$";
    return match_regex(pattern, input) ? "Valid phone number" : "Invalid phone number";
}

const char* validate_password(const char* input) {
    int length = strlen(input);
    if (length < 8) return "Invalid password";

    int has_lower = 0, has_upper = 0, has_digit = 0;
    for (int i = 0; i < length; i++) {
        if (input[i] >= 'a' && input[i] <= 'z') has_lower = 1;
        else if (input[i] >= 'A' && input[i] <= 'Z') has_upper = 1;
        else if (input[i] >= '0' && input[i] <= '9') has_digit = 1;
        else return "Invalid password"; // رموز غير مسموحة
    }

    return (has_lower && has_upper && has_digit) ? "Valid password" : "Invalid password";
}

// دالة اختبار واحدة للطباعة pass/fail
void test_case(const char* test_name, const char* (*func)(const char*), const char* input, const char* expected) {
    const char* result = func(input);
    if (strcmp(result, expected) == 0) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected '%s', got '%s')\n", test_name, expected, result);
    }
}

int main() {
    // اختبار التحقق من الإيميل
    test_case("test_valid_email", validate_email, "test@example.com", "Valid email");
    test_case("test_invalid_email", validate_email, "invalid-email", "Invalid email");
    test_case("test_valid_email_with_subdomain", validate_email, "user@domain.co.uk", "Valid email");
    test_case("test_invalid_email_missing_username", validate_email, "user@.com", "Invalid email");
    test_case("test_invalid_email_missing_at_symbol", validate_email, "userdomain.com", "Invalid email");

    // اختبار التحقق من رقم الهاتف
    test_case("test_valid_phone", validate_phone, "(123) 456-7890", "Valid phone number");
    test_case("test_invalid_phone_with_letters", validate_phone, "123-abc-7890", "Invalid phone number");
    test_case("test_valid_phone_with_dashes", validate_phone, "123-456-7890", "Valid phone number");
    test_case("test_invalid_phone_with_extra_digits", validate_phone, "123-456-78901", "Invalid phone number");

    // اختبار التحقق من كلمة المرور
    test_case("test_valid_password", validate_password, "Password1", "Valid password");
    test_case("test_invalid_password_no_uppercase", validate_password, "password1", "Invalid password");
    test_case("test_invalid_password_no_digit", validate_password, "Password", "Invalid password");
    test_case("test_invalid_password_too_short", validate_password, "Pass1", "Invalid password");

    return 0;
}
