#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PASSWORD_LENGTH 100

bool isValidAdminPassword(const char* password) {
    int length = strlen(password);
    if (length < 12) {
        return false;
    }

    bool hasUppercase = false;
    bool hasLowercase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;

    for (int i = 0; i < length; i++) {
        if (isupper((unsigned char)password[i])) hasUppercase = true;
        if (islower((unsigned char)password[i])) hasLowercase = true;
        if (isdigit((unsigned char)password[i])) hasDigit = true;
        if (strchr("!@#$%^&*(),.?\":{}|<>", password[i])) hasSpecialChar = true;
    }

    if (!hasUppercase || !hasLowercase || !hasDigit || !hasSpecialChar) {
        return false;
    }

    char lowercasePassword[MAX_PASSWORD_LENGTH];
    strncpy(lowercasePassword, password, MAX_PASSWORD_LENGTH - 1);
    lowercasePassword[MAX_PASSWORD_LENGTH - 1] = '\0';

    for (int i = 0; i < length && i < MAX_PASSWORD_LENGTH - 1; i++) {
        lowercasePassword[i] = tolower((unsigned char)lowercasePassword[i]);
    }

    if (strstr(lowercasePassword, "admin") != NULL) {
        return false;
    }

    return true;
}

// Test cases strict from Python
#include <unistd.h>  // pipe, dup2, read, close

void print_result(const char* test_name, bool passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

int capture_output(bool (*test_func)(void), const char* test_name) {
    // Just run the test_func and print result, no stdout capture needed here
    bool passed = test_func();
    print_result(test_name, passed);
    return 0;
}

// Test implementations:

bool test_valid_password() {
    return isValidAdminPassword("Valid1Password$2024") == true;
}
bool test_too_short_password() {
    return isValidAdminPassword("Short1$") == false;
}
bool test_missing_uppercase() {
    return isValidAdminPassword("validpassword1$") == false;
}
bool test_missing_lowercase() {
    return isValidAdminPassword("VALIDPASSWORD1$") == false;
}
bool test_missing_digit() {
    return isValidAdminPassword("ValidPassword$") == false;
}
bool test_missing_special_char() {
    return isValidAdminPassword("ValidPassword1") == false;
}
bool test_contains_admin() {
    return isValidAdminPassword("Admin1234$") == false;
}
bool test_valid_password_with_special_chars() {
    return isValidAdminPassword("Valid1Password@#") == true;
}
bool test_contains_admin_case_insensitive() {
    return isValidAdminPassword("superadmin2024$") == false;
}
bool test_valid_password_with_longer_length() {
    return isValidAdminPassword("VeryLongValidPassword1$2024") == true;
}

int main() {
    capture_output(test_valid_password, "test_valid_password");
    capture_output(test_too_short_password, "test_too_short_password");
    capture_output(test_missing_uppercase, "test_missing_uppercase");
    capture_output(test_missing_lowercase, "test_missing_lowercase");
    capture_output(test_missing_digit, "test_missing_digit");
    capture_output(test_missing_special_char, "test_missing_special_char");
    capture_output(test_contains_admin, "test_contains_admin");
    capture_output(test_valid_password_with_special_chars, "test_valid_password_with_special_chars");
    capture_output(test_contains_admin_case_insensitive, "test_contains_admin_case_insensitive");
    capture_output(test_valid_password_with_longer_length, "test_valid_password_with_longer_length");
    return 0;
}
