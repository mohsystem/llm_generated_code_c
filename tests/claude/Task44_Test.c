#include <stdio.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 33

static char csrf_token[MAX_TOKEN_LENGTH];

// Fixed token for reproducibility
void generate_csrf_token() {
    // Same fixed token for all tests
    strcpy(csrf_token, "fixed_test_token_1234567890abcdef");
}

// Returns 1 if token matches, else 0
int validate_csrf_token(const char *received_token) {
    if (received_token == NULL || strlen(received_token) == 0)
        return 0;
    return (strcmp(received_token, csrf_token) == 0);
}

void run_test(const char *post_token, int expected_result, const char *test_name) {
    int result = validate_csrf_token(post_token);
    if (result == expected_result) {
        printf("%s: PASS\n", test_name);
    } else {
        printf("%s: FAIL\n", test_name);
    }
}

int main() {
    generate_csrf_token();

    // Test cases exactly matching Python logic:

    // 1. Correct token (should pass)
    run_test(csrf_token, 1, "test_valid_token");

    // 2. Wrong token (should fail)
    run_test("wrongtoken123", 0, "test_wrong_token");

    // 3. Null token (should fail)
    run_test(NULL, 0, "test_null_token");

    // 4. Empty string token (should fail)
    run_test("", 0, "test_empty_token");

    // 5. Token with special characters (wrong token - fail)
    run_test("!@#$%^&*()", 0, "test_special_chars_token");

    // 6. Token with correct prefix but extra chars (fail)
    run_test("fixed_test_token_1234567890abcdef_extra", 0, "test_token_with_extra");

    return 0;
}
