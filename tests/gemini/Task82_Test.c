#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

// الدالة كما هي (salt + password)
void hash_password(const char* password, const char* salt, char* output) {
  char salted_password[256];
  snprintf(salted_password, sizeof(salted_password), "%s%s", salt, password);

  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, salted_password, strlen(salted_password));
  SHA256_Final(hash, &sha256);

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(output + (i * 2), "%02x", hash[i]);
  }
  output[64] = 0;
}

// دالة لحساب هاش password + salt (للتست كيسز)
void compute_expected_hash_password_salt(const char* password, const char* salt, char* output) {
    char combined[256];
    snprintf(combined, sizeof(combined), "%s%s", password, salt);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, combined, strlen(combined));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = 0;
}

// دالة مقارنة نصوص
int string_equal(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

int main() {
    char result[65];
    char expected[65];

    const char* password = "mypassword123";
    const char* salt = "randomsalt";

    int pass_count = 0, fail_count = 0;

    // -------- test_hash_password_standard_case --------
    hash_password(password, salt, result);
    compute_expected_hash_password_salt(password, salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_standard_case: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_standard_case: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_empty_password --------
    hash_password("", salt, result);
    compute_expected_hash_password_salt("", salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_password: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_empty_salt --------
    hash_password(password, "", result);
    compute_expected_hash_password_salt(password, "", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_salt: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_empty_password_and_salt --------
    hash_password("", "", result);
    compute_expected_hash_password_salt("", "", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password_and_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_password_and_salt: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_different_password --------
    hash_password("differentpassword", salt, result);
    if(!string_equal(result, expected)) { // expected is previous hash for mypassword123+randomsalt
        printf("test_hash_password_different_password: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_password: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_different_salt --------
    hash_password(password, "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_salt: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_different_password_and_salt --------
    hash_password("differentpassword", "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_password_and_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_password_and_salt: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_with_special_characters --------
    hash_password("passw@rd!$", "s@lt#", result);
    char combined_special[256];
    snprintf(combined_special, sizeof(combined_special), "%s%s", "passw@rd!$", "s@lt#");
    compute_expected_hash_password_salt("passw@rd!$", "s@lt#", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_special_characters: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_with_special_characters: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_with_unicode_characters --------
    hash_password("p@sswørd", "råndøm$alt", result);
    compute_expected_hash_password_salt("p@sswørd", "råndøm$alt", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_unicode_characters: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_with_unicode_characters: FAIL\n");
        fail_count++;
    }

    // -------- test_hash_password_long_string --------
    char long_password[1001];
    char long_salt[1001];
    for(int i=0; i<1000; i++) {
        long_password[i] = 'a';
        long_salt[i] = 'b';
    }
    long_password[1000] = '\0';
    long_salt[1000] = '\0';

    hash_password(long_password, long_salt, result);
    compute_expected_hash_password_salt(long_password, long_salt, expected);
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
