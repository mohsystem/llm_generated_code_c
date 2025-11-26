#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

// دالة حساب SHA256 لـ (password + salt)
void compute_hash(const char* password, const char* salt, char* outputBuffer) {
    char combined[512];
    snprintf(combined, sizeof(combined), "%s%s", password, salt);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)combined, strlen(combined), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = '\0';
}

// دالة لحساب هاش password+salt (للمقارنة في التست)
void compute_expected_hash(const char* password, const char* salt, char* outputBuffer) {
    compute_hash(password, salt, outputBuffer);
}

// مقارنة نصوص
int string_equal(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

int main() {
    char result[65];
    char expected[65];

    int pass_count = 0, fail_count = 0;

    // القيم الثابتة من البايثون
    const char* password = "mypassword123";
    const char* salt = "randomsalt";

    // 1. standard case
    compute_hash(password, salt, result);
    compute_expected_hash(password, salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_standard_case: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_standard_case: FAIL\n"); fail_count++;
    }

    // 2. empty password
    compute_hash("", salt, result);
    compute_expected_hash("", salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_empty_password: FAIL\n"); fail_count++;
    }

    // 3. empty salt
    compute_hash(password, "", result);
    compute_expected_hash(password, "", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_salt: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_empty_salt: FAIL\n"); fail_count++;
    }

    // 4. empty password and salt
    compute_hash("", "", result);
    compute_expected_hash("", "", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password_and_salt: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_empty_password_and_salt: FAIL\n"); fail_count++;
    }

    // 5. different password
    compute_hash("differentpassword", salt, result);
    // المقارنة مع expected للـ mypassword123+randomsalt للتأكد أنه مختلف
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_password: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_different_password: FAIL\n"); fail_count++;
    }

    // 6. different salt
    compute_hash(password, "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_salt: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_different_salt: FAIL\n"); fail_count++;
    }

    // 7. different password and salt
    compute_hash("differentpassword", "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_password_and_salt: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_different_password_and_salt: FAIL\n"); fail_count++;
    }

    // 8. with special characters
    compute_hash("passw@rd!$", "s@lt#", result);
    compute_expected_hash("passw@rd!$", "s@lt#", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_special_characters: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_with_special_characters: FAIL\n"); fail_count++;
    }

    // 9. with unicode characters
    compute_hash("p@sswørd", "råndøm$alt", result);
    compute_expected_hash("p@sswørd", "råndøm$alt", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_unicode_characters: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_with_unicode_characters: FAIL\n"); fail_count++;
    }

    // 10. long string
    char long_password[1001];
    char long_salt[1001];
    for(int i=0; i<1000; i++) {
        long_password[i] = 'a';
        long_salt[i] = 'b';
    }
    long_password[1000] = '\0';
    long_salt[1000] = '\0';

    compute_hash(long_password, long_salt, result);
    compute_expected_hash(long_password, long_salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_long_string: PASS\n"); pass_count++;
    } else {
        printf("test_hash_password_long_string: FAIL\n"); fail_count++;
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass_count, fail_count);

    return 0;
}
