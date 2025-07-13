#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

void computeHash(const char* password, const char* salt, char* output) {
    char combined[strlen(password) + strlen(salt) + 1];
    strcpy(combined, password);
    strcat(combined, salt);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)combined, strlen(combined), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + i * 2, "%02x", hash[i]);
    }
    output[SHA256_DIGEST_LENGTH * 2] = '\0';
}

int string_equal(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

int main() {
    char result[SHA256_DIGEST_LENGTH * 2 + 1];
    char expected[SHA256_DIGEST_LENGTH * 2 + 1];

    const char* password = "mypassword123";
    const char* salt = "randomsalt";

    int pass_count = 0, fail_count = 0;

    // 1. test_hash_password_standard_case
    computeHash(password, salt, result);
    computeHash(password, salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_standard_case: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_standard_case: FAIL\n");
        fail_count++;
    }

    // 2. test_hash_password_empty_password
    computeHash("", salt, result);
    computeHash("", salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_password: FAIL\n");
        fail_count++;
    }

    // 3. test_hash_password_empty_salt
    computeHash(password, "", result);
    computeHash(password, "", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_salt: FAIL\n");
        fail_count++;
    }

    // 4. test_hash_password_empty_password_and_salt
    computeHash("", "", result);
    computeHash("", "", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password_and_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_password_and_salt: FAIL\n");
        fail_count++;
    }

    // 5. test_hash_password_different_password
    computeHash("differentpassword", salt, result);
    if(!string_equal(result, expected)) { // expected is hash for mypassword123+randomsalt
        printf("test_hash_password_different_password: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_password: FAIL\n");
        fail_count++;
    }

    // 6. test_hash_password_different_salt
    computeHash(password, "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_salt: FAIL\n");
        fail_count++;
    }

    // 7. test_hash_password_different_password_and_salt
    computeHash("differentpassword", "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_password_and_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_password_and_salt: FAIL\n");
        fail_count++;
    }

    // 8. test_hash_password_with_special_characters
    computeHash("passw@rd!$", "s@lt#", result);
    computeHash("passw@rd!$", "s@lt#", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_special_characters: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_with_special_characters: FAIL\n");
        fail_count++;
    }

    // 9. test_hash_password_with_unicode_characters
    computeHash("p@sswørd", "råndøm$alt", result);
    computeHash("p@sswørd", "råndøm$alt", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_unicode_characters: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_with_unicode_characters: FAIL\n");
        fail_count++;
    }

    // 10. test_hash_password_long_string
    char long_password[1001];
    char long_salt[1001];
    for(int i = 0; i < 1000; i++) {
        long_password[i] = 'a';
        long_salt[i] = 'b';
    }
    long_password[1000] = '\0';
    long_salt[1000] = '\0';

    computeHash(long_password, long_salt, result);
    computeHash(long_password, long_salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_long_string: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_long_string: FAIL\n");
        fail_count++;
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass_count, fail_count);

    return 0;
}
