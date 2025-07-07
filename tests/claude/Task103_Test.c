#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to check the strength of the password
int checkPasswordStrength(const char* password) {
    int length = strlen(password);
    int hasUpper = 0, hasLower = 0, hasDigit = 0;

    // Check if the password is at least 8 characters long
    if (length < 8) {
        return 0;
    }

    // Check if the password contains uppercase, lowercase, and digit
    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) {
            hasUpper = 1;
        } else if (islower(password[i])) {
            hasLower = 1;
        } else if (isdigit(password[i])) {
            hasDigit = 1;
        }
    }

    // The password is strong if it has all the required criteria
    return hasUpper && hasLower && hasDigit;
}

void test_case(const char* password, int expected) {
    int result = checkPasswordStrength(password);
    if (result == expected) {
        printf("Test Passed: \"%s\" -> %d\n", password, result);
    } else {
        printf("Test Failed: \"%s\" -> %d (Expected: %d)\n", password, result, expected);
    }
}

int main() {
    // Run all test cases
    test_case("Ab1", 0);  // Test Case 1: Short password, should fail
    test_case("abcdefgh1", 0);  // Test Case 2: No uppercase, should fail
    test_case("ABCDEF1", 0);  // Test Case 3: No lowercase, should fail
    test_case("Abcdefgh", 0);  // Test Case 4: No digit, should fail
    test_case("Abcdefg1", 1);  // Test Case 5: Valid password, should pass
    test_case("A1bcdefg", 1);  // Test Case 6: Exactly 8 characters, should pass
    test_case("A1@bcdEf", 1);  // Test Case 7: Password with special characters, should pass
    test_case("ABCDEFGHI", 0);  // Test Case 8: All uppercase, should fail
    test_case("abcdefgh", 0);  // Test Case 9: All lowercase, should fail
    test_case("A1 bcdEf", 1);  // Test Case 10: Password with spaces, should pass

    return 0;
}
