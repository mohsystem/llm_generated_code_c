#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> // pipe, dup2, read, close

int checkPassword(const char* password) {
    if (strlen(password) < 8) {
        return 0;
    }
    int hasDigit = 0;
    int hasUpper = 0;
    int hasLower = 0;
    for (int i = 0; password[i] != '\0'; i++) {
        if (isdigit((unsigned char)password[i])) {
            hasDigit = 1;
        }
        if (isupper((unsigned char)password[i])) {
            hasUpper = 1;
        }
        if (islower((unsigned char)password[i])) {
            hasLower = 1;
        }
    }
    return hasDigit && hasUpper && hasLower;
}

void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        return;
    }
    if (checkPassword(argv[1])) {
        printf("Password is valid\n");
    } else {
        printf("Password is not valid\n");
    }
}

void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

int capture_output(void (*func)(int,char**), int argc, char* argv[],
                   char* out_buf, size_t out_size,
                   char* err_buf, size_t err_size) {
    int out_pipe[2], err_pipe[2];
    if (pipe(out_pipe) == -1) return -1;
    if (pipe(err_pipe) == -1) return -1;

    fflush(stdout);
    fflush(stderr);

    int stdout_copy = dup(fileno(stdout));
    int stderr_copy = dup(fileno(stderr));
    if (stdout_copy == -1 || stderr_copy == -1) return -1;

    dup2(out_pipe[1], fileno(stdout));
    dup2(err_pipe[1], fileno(stderr));
    close(out_pipe[1]);
    close(err_pipe[1]);

    func(argc, argv);

    fflush(stdout);
    fflush(stderr);

    dup2(stdout_copy, fileno(stdout));
    dup2(stderr_copy, fileno(stderr));
    close(stdout_copy);
    close(stderr_copy);

    ssize_t out_len = read(out_pipe[0], out_buf, out_size - 1);
    ssize_t err_len = read(err_pipe[0], err_buf, err_size - 1);
    close(out_pipe[0]);
    close(err_pipe[0]);

    if (out_len < 0 || err_len < 0) return -1;

    out_buf[out_len] = '\0';
    err_buf[err_len] = '\0';

    return 0;
}

// Test cases from Python strict

int test_valid_password() {
    char* argv[] = {"program", "Valid1Password$2024"};
    char out[1024], err[1024];
    int ret = capture_output(program_main, 2, argv, out, sizeof(out), err, sizeof(err));
    // This code doesn't check special chars or "admin", so likely FAIL
    // But we test just digit, upper, lower and length >= 8
    // "Valid1Password$2024" contains digit, upper, lower, length>8 => PASS
    return ret == 0 && strstr(out, "Password is valid") != NULL;
}

int test_too_short_password() {
    char* argv[] = {"program", "Short1$"};
    char out[1024], err[1024];
    int ret = capture_output(program_main, 2, argv, out, sizeof(out), err, sizeof(err));
    // Length < 8, should be invalid
    return ret == 0 && strstr(out, "not valid") != NULL;
}

int test_missing_uppercase() {
    char* argv[] = {"program", "validpassword1$"};
    char out[1024], err[1024];
    int ret = capture_output(program_main, 2, argv, out, sizeof(out), err, sizeof(err));
    // No uppercase, should be invalid
    return ret == 0 && strstr(out, "not valid") != NULL;
}

int test_missing_lowercase() {
    char* argv[] = {"program", "VALIDPASSWORD1$"};
    char out[1024], err[1024];
    int ret = capture_output(program_main, 2, argv, out, sizeof(out), err, sizeof(err));
    // No lowercase, invalid
    return ret == 0 && strstr(out, "not valid") != NULL;
}

int test_missing_digit() {
    char* argv[] = {"program", "ValidPassword$"};
    char out[1024], err[1024];
    int ret = capture_output(program_main, 2, argv, out, sizeof(out), err, sizeof(err));
    // No digit, invalid (our C code checks digits)
    return ret == 0 && strstr(out, "not valid") != NULL;
}

int test_missing_special_char() {
    char* argv[] = {"program", "ValidPassword1"};
    // The C code does NOT check special chars, so this will be VALID wrongly
    // So we print fail not implemented
    return 0; // FAIL: not implemented
}

int test_contains_admin() {
    char* argv[] = {"program", "Admin1234$"};
    // C code doesn't check "admin" substring -> fail not implemented
    return 0;
}

int test_valid_password_with_special_chars() {
    char* argv[] = {"program", "Valid1Password@#"};
    // C code ignores special char check -> fail not implemented
    return 0;
}

int test_contains_admin_case_insensitive() {
    char* argv[] = {"program", "superadmin2024$"};
    // C code doesn't check admin substring -> fail not implemented
    return 0;
}

int test_valid_password_with_longer_length() {
    char* argv[] = {"program", "VeryLongValidPassword1$2024"};
    // C code ignores special char and admin substring -> fail not implemented
    return 0;
}

int main() {
    print_result("test_valid_password", test_valid_password());
    print_result("test_too_short_password", test_too_short_password());
    print_result("test_missing_uppercase", test_missing_uppercase());
    print_result("test_missing_lowercase", test_missing_lowercase());
    print_result("test_missing_digit", test_missing_digit());
    print_result("test_missing_special_char", test_missing_special_char());
    print_result("test_contains_admin", test_contains_admin());
    print_result("test_valid_password_with_special_chars", test_valid_password_with_special_chars());
    print_result("test_contains_admin_case_insensitive", test_contains_admin_case_insensitive());
    print_result("test_valid_password_with_longer_length", test_valid_password_with_longer_length());
    return 0;
}
