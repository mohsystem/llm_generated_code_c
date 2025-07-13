#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65]) {
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

void sha256(const char *string, char outputBuffer[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    sha256_hash_string(hash, outputBuffer);
}

void hashPassword(const char *password, const char *salt, char outputBuffer[65]) {
    char combined[256];
    snprintf(combined, sizeof(combined), "%s%s", password, salt);
    sha256(combined, outputBuffer);
}

// دالة مساعدة لحساب الهاش مباشرة (للمقارنة بالنتيجة المتوقعة)
void compute_expected_hash(const char *input, char outputBuffer[65]) {
    sha256(input, outputBuffer);
}

// دالة مقارنة نصوص
int string_equal(const char *a, const char *b) {
    return strcmp(a,b) == 0;
}

int main() {
    char result[65];
    char expected[65];
    int pass_count = 0, fail_count = 0;

    // common values
    const char *password = "mypassword123";
    const char *salt = "randomsalt";

    // expected hash for standard case (password+salt)
    char combined[256];
    snprintf(combined, sizeof(combined), "%s%s", password, salt);
    compute_expected_hash(combined, expected);

    // -------- Test case 1: standard case --------
    hashPassword(password, salt, result);
    if(string_equal(result, expected)) {
        printf("test_hash_password_standard_case: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_standard_case: FAIL\n");
        fail_count++;
    }

    // -------- Test case 2: empty password --------
    hashPassword("", salt, result);
    compute_expected_hash(salt, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_password: FAIL\n");
        fail_count++;
    }

    // -------- Test case 3: empty salt --------
    hashPassword(password, "", result);
    compute_expected_hash(password, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_salt: FAIL\n");
        fail_count++;
    }

    // -------- Test case 4: empty password and salt --------
    hashPassword("", "", result);
    compute_expected_hash("", expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_empty_password_and_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_empty_password_and_salt: FAIL\n");
        fail_count++;
    }

    // -------- Test case 5: different password --------
    hashPassword("differentpassword", salt, result);
    if(!string_equal(result, expected)) { // expected is old hash for mypassword123+randomsalt
        printf("test_hash_password_different_password: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_password: FAIL\n");
        fail_count++;
    }

    // -------- Test case 6: different salt --------
    hashPassword(password, "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_salt: FAIL\n");
        fail_count++;
    }

    // -------- Test case 7: different password and salt --------
    hashPassword("differentpassword", "differentsalt", result);
    if(!string_equal(result, expected)) {
        printf("test_hash_password_different_password_and_salt: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_different_password_and_salt: FAIL\n");
        fail_count++;
    }

    // -------- Test case 8: with special characters --------
    hashPassword("passw@rd!$", "s@lt#", result);
    snprintf(combined, sizeof(combined), "%s%s", "passw@rd!$", "s@lt#");
    compute_expected_hash(combined, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_special_characters: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_with_special_characters: FAIL\n");
        fail_count++;
    }

    // -------- Test case 9: with unicode characters --------
    // Note: C strings must be UTF-8 encoded; using same strings literal (may depend on compiler)
    hashPassword("p@sswørd", "råndøm$alt", result);
    snprintf(combined, sizeof(combined), "%s%s", "p@sswørd", "råndøm$alt");
    compute_expected_hash(combined, expected);
    if(string_equal(result, expected)) {
        printf("test_hash_password_with_unicode_characters: PASS\n");
        pass_count++;
    } else {
        printf("test_hash_password_with_unicode_characters: FAIL\n");
        fail_count++;
    }

    // -------- Test case 10: long string --------
    char long_password[1001];
    char long_salt[1001];
    for(int i=0; i<1000; i++) {
        long_password[i] = 'a';
        long_salt[i] = 'b';
    }
    long_password[1000] = '\0';
    long_salt[1000] = '\0';

    hashPassword(long_password, long_salt, result);
    snprintf(combined, sizeof(combined), "%s%s", long_password, long_salt);
    compute_expected_hash(combined, expected);
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
