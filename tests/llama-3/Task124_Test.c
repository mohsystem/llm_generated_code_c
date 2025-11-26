#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char data[20];
} SensitiveData;

SensitiveData* createSensitiveData(char* data) {
    SensitiveData* sd = (SensitiveData*) malloc(sizeof(SensitiveData));
    strcpy(sd->data, data);
    return sd;
}

char* getData(SensitiveData* sd) {
    return sd->data;
}

// === Test framework ===
#define ASSERT_STR_EQUAL(a, b, testname) do { \
    if (strcmp(a, b) == 0) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_EXCEPTION(testname) do { \
    printf("%s: FAIL (Not Implemented)\n", testname); \
} while(0)

int main() {
    // --- test_encrypt_credit_card ---
    {
        SensitiveData* sd = createSensitiveData("1234-5678-9012-3456");
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, "1234-5678-9012-3456", "test_encrypt_credit_card");
        free(sd);
    }

    // --- test_encrypt_ssn ---
    {
        SensitiveData* sd = createSensitiveData("123-45-6789");
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, "123-45-6789", "test_encrypt_ssn");
        free(sd);
    }

    // --- test_encrypt_password ---
    {
        SensitiveData* sd = createSensitiveData("mySecretPass123");
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, "mySecretPass123", "test_encrypt_password");
        free(sd);
    }

    // --- test_encrypt_empty_string ---
    {
        SensitiveData* sd = createSensitiveData("");
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, "", "test_encrypt_empty_string");
        free(sd);
    }

    // --- test_encrypt_special_characters ---
    {
        SensitiveData* sd = createSensitiveData("!@#$%^&*()_+-=<>?");
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, "!@#$%^&*()_+-=<>?", "test_encrypt_special_characters");
        free(sd);
    }

    // --- test_decrypt_empty_string ---
    {
        SensitiveData* sd = createSensitiveData("");
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, "", "test_decrypt_empty_string");
        free(sd);
    }

    // --- test_decrypt_invalid_base64 ---
    ASSERT_EXCEPTION("test_decrypt_invalid_base64");

    // --- test_encrypt_decrypt_with_long_string ---
    {
        char longstr[21];
        for (int i = 0; i < 20; i++) longstr[i] = 'A';
        longstr[20] = '\0';
        SensitiveData* sd = createSensitiveData(longstr);
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, longstr, "test_encrypt_decrypt_with_long_string");
        free(sd);
    }

    // --- test_encrypt_decrypt_with_numeric_string ---
    {
        SensitiveData* sd = createSensitiveData("1234567890");
        char* data = getData(sd);
        ASSERT_STR_EQUAL(data, "1234567890", "test_encrypt_decrypt_with_numeric_string");
        free(sd);
    }

    // --- test_encrypt_decrypt_with_unicode ---
    ASSERT_EXCEPTION("test_encrypt_decrypt_with_unicode");

    return 0;
}
