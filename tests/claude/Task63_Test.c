#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <jansson.h>

json_t* process_request(const char* raw_data) {
    // في C نفترض أن raw_data مُرمز UTF-8 مسبقاً

    BIO *b64, *bmem;
    char *decoded_data;
    int raw_len = strlen(raw_data);
    int decoded_length;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new_mem_buf(raw_data, raw_len);
    bmem = BIO_push(b64, bmem);

    decoded_data = malloc(raw_len);
    if (!decoded_data) {
        BIO_free_all(bmem);
        return NULL;
    }

    decoded_length = BIO_read(bmem, decoded_data, raw_len);
    BIO_free_all(bmem);

    if (decoded_length <= 0) {
        free(decoded_data);
        return NULL;
    }
    decoded_data[decoded_length] = '\0';  // تأكد من انتهاء النص

    json_error_t error;
    json_t* deserialized_data = json_loads(decoded_data, 0, &error);

    free(decoded_data);

    return deserialized_data;
}

int main() {
    struct {
        const char* raw_input;
        int expect_success;  // 1 = نتوقع نجاح، 0 = نتوقع فشل (NULL أو خطأ)
        const char* test_name;
    } tests[] = {
        {"eyJuYW1lIjogIkpvaG4gRG9lIiwgImFnZSI6IDMwfQ==", 1, "test_valid_json"},
        {"", 0, "test_empty_input"},
        {"invalid_base64_string", 0, "test_invalid_base64"},
        {"bmFtZSI6ICJKb2hufQ==", 0, "test_malformed_json"},  // Base64 لـ 'name": "John}' JSON خاطئ
        {"e30=", 1, "test_base64_of_empty_json"},             // "{}"
        {"eyJrZXkiOiAidmFsdWVfd2l0aF9zcGVjaWFsX2NoYXJhY3RlcnNfIUAjJCVeJiooKSJ9", 1, "test_json_with_special_characters"},
        {"eyJ2YWx1ZSI6IDEyMzR9", 1, "test_json_with_numeric_values"},
        {"eyJhY3RpdmUiOiB0cnVlLCAidmVyaWZpZWQiOiBmYWxzZX0=", 1, "test_json_with_boolean_values"},
        {"eyJ2YWx1ZSI6IG51bGx9", 1, "test_json_with_null_value"},
        {"eyJvdXRlciI6IHsiaW5uZXIiOiAidmFsdWUifX0=", 1, "test_nested_json"}
    };

    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        json_t* result = process_request(tests[i].raw_input);
        int success = (result != NULL) ? 1 : 0;

        if (success == tests[i].expect_success) {
            printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
            pass++;
        } else {
            printf("Test %d (%s): FAIL\n", i + 1, tests[i].test_name);
            fail++;
        }

        if (result != NULL) {
            json_decref(result);
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
