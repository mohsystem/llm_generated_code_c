#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DATA 3
#define MAX_LENGTH 50

void encrypt(char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] = data[i] + 1;
    }
}

void decrypt(char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] = data[i] - 1;
    }
}

// === Test framework ===
#define ASSERT_STR_EQUAL(a, b, testname) do { \
    if (strcmp(a, b) == 0) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_EXCEPTION(testname) do { \
    printf("%s: FAIL (Not Implemented - exception test)\n", testname); \
} while(0)

int main() {
    // Original data
    char keys[MAX_DATA][MAX_LENGTH] = {"credit_card", "ssn", "password"};
    char values[MAX_DATA][MAX_LENGTH] = {"1234-5678-9012-3456", "123-45-6789", "mySecretPass123"};

    // Buffers for encryption/decryption
    char encrypted[MAX_DATA][MAX_LENGTH];
    char decrypted[MAX_DATA][MAX_LENGTH];

    // --- test_encrypt_credit_card ---
    strcpy(encrypted[0], values[0]);
    encrypt(encrypted[0]);
    strcpy(decrypted[0], encrypted[0]);
    decrypt(decrypted[0]);
    ASSERT_STR_EQUAL(decrypted[0], values[0], "test_encrypt_credit_card");

    // --- test_encrypt_ssn ---
    strcpy(encrypted[1], values[1]);
    encrypt(encrypted[1]);
    strcpy(decrypted[1], encrypted[1]);
    decrypt(decrypted[1]);
    ASSERT_STR_EQUAL(decrypted[1], values[1], "test_encrypt_ssn");

    // --- test_encrypt_password ---
    strcpy(encrypted[2], values[2]);
    encrypt(encrypted[2]);
    strcpy(decrypted[2], encrypted[2]);
    decrypt(decrypted[2]);
    ASSERT_STR_EQUAL(decrypted[2], values[2], "test_encrypt_password");

    // --- test_encrypt_empty_string ---
    char empty[] = "";
    char empty_enc[MAX_LENGTH];
    char empty_dec[MAX_LENGTH];
    strcpy(empty_enc, empty);
    encrypt(empty_enc);
    strcpy(empty_dec, empty_enc);
    decrypt(empty_dec);
    ASSERT_STR_EQUAL(empty_dec, empty, "test_encrypt_empty_string");

    // --- test_encrypt_special_characters ---
    char special[] = "!@#$%^&*()_+-=<>?";
    char special_enc[MAX_LENGTH];
    char special_dec[MAX_LENGTH];
    strcpy(special_enc, special);
    encrypt(special_enc);
    strcpy(special_dec, special_enc);
    decrypt(special_dec);
    ASSERT_STR_EQUAL(special_dec, special, "test_encrypt_special_characters");

    // --- test_decrypt_empty_string ---
    // Same as test_encrypt_empty_string
    ASSERT_STR_EQUAL(empty_dec, empty, "test_decrypt_empty_string");

    // --- test_decrypt_invalid_base64 ---
    // Not implemented because C code does not handle exceptions or base64
    ASSERT_EXCEPTION("test_decrypt_invalid_base64");

    // --- test_encrypt_decrypt_with_long_string ---
    char long_str[1001];
    for (int i = 0; i < 1000; i++) long_str[i] = 'A';
    long_str[1000] = '\0';
    char long_enc[1001];
    char long_dec[1001];
    strcpy(long_enc, long_str);
    encrypt(long_enc);
    strcpy(long_dec, long_enc);
    decrypt(long_dec);
    ASSERT_STR_EQUAL(long_dec, long_str, "test_encrypt_decrypt_with_long_string");

    // --- test_encrypt_decrypt_with_numeric_string ---
    char numeric[] = "1234567890";
    char numeric_enc[MAX_LENGTH];
    char numeric_dec[MAX_LENGTH];
    strcpy(numeric_enc, numeric);
    encrypt(numeric_enc);
    strcpy(numeric_dec, numeric_enc);
    decrypt(numeric_dec);
    ASSERT_STR_EQUAL(numeric_dec, numeric, "test_encrypt_decrypt_with_numeric_string");

    // --- test_encrypt_decrypt_with_unicode ---
    // Not implemented: C code does not handle unicode or multibyte strings properly
    ASSERT_EXCEPTION("test_encrypt_decrypt_with_unicode");

    return 0;
}
