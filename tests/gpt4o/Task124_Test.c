#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SensitiveData {
    char *key;
    char *value;
    struct SensitiveData *next;
} SensitiveData;

SensitiveData* createNode(const char *key, const char *value) {
    SensitiveData *newNode = (SensitiveData*) malloc(sizeof(SensitiveData));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = NULL;
    return newNode;
}

void storeData(SensitiveData **head, const char *key, const char *value) {
    SensitiveData *newNode = createNode(key, value);
    newNode->next = *head;
    *head = newNode;
}

const char* retrieveData(SensitiveData *head, const char *key) {
    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
            return head->value;
        }
        head = head->next;
    }
    return "Data not found";
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
    SensitiveData *head = NULL;

    // Store initial data (like in your main)
    storeData(&head, "credit_card", "1234-5678-9012-3456");
    storeData(&head, "ssn", "123-45-6789");
    storeData(&head, "password", "mySecretPass123");

    // --- test_encrypt_credit_card ---
    {
        const char* data = retrieveData(head, "credit_card");
        ASSERT_STR_EQUAL(data, "1234-5678-9012-3456", "test_encrypt_credit_card");
    }

    // --- test_encrypt_ssn ---
    {
        const char* data = retrieveData(head, "ssn");
        ASSERT_STR_EQUAL(data, "123-45-6789", "test_encrypt_ssn");
    }

    // --- test_encrypt_password ---
    {
        const char* data = retrieveData(head, "password");
        ASSERT_STR_EQUAL(data, "mySecretPass123", "test_encrypt_password");
    }

    // --- test_encrypt_empty_string ---
    {
        // Empty string key/value test - store and retrieve
        storeData(&head, "empty", "");
        const char* data = retrieveData(head, "empty");
        ASSERT_STR_EQUAL(data, "", "test_encrypt_empty_string");
    }

    // --- test_encrypt_special_characters ---
    {
        const char* special = "!@#$%^&*()_+-=<>?";
        storeData(&head, "special_chars", special);
        const char* data = retrieveData(head, "special_chars");
        ASSERT_STR_EQUAL(data, special, "test_encrypt_special_characters");
    }

    // --- test_decrypt_empty_string ---
    {
        const char* data = retrieveData(head, "empty");
        ASSERT_STR_EQUAL(data, "", "test_decrypt_empty_string");
    }

    // --- test_decrypt_invalid_base64 ---
    // No base64 decoding or exception handling in your code
    ASSERT_EXCEPTION("test_decrypt_invalid_base64");

    // --- test_encrypt_decrypt_with_long_string ---
    {
        char long_str[1001];
        for (int i = 0; i < 1000; i++) long_str[i] = 'A';
        long_str[1000] = '\0';
        storeData(&head, "long_string", long_str);
        const char* data = retrieveData(head, "long_string");
        ASSERT_STR_EQUAL(data, long_str, "test_encrypt_decrypt_with_long_string");
    }

    // --- test_encrypt_decrypt_with_numeric_string ---
    {
        storeData(&head, "numeric_string", "1234567890");
        const char* data = retrieveData(head, "numeric_string");
        ASSERT_STR_EQUAL(data, "1234567890", "test_encrypt_decrypt_with_numeric_string");
    }

    // --- test_encrypt_decrypt_with_unicode ---
    // Unicode handling not supported, plain C strings only
    ASSERT_EXCEPTION("test_encrypt_decrypt_with_unicode");

    // Free linked list memory (optional, not required for tests)
    SensitiveData *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }

    return 0;
}
