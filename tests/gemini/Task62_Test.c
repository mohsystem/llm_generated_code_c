#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h" // تأكد أن المكتبة موجودة ومسارها صحيح

cJSON* getRootElement(const char* jsonString) {
    cJSON* root = cJSON_Parse(jsonString);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error parsing JSON: %s\n", error_ptr);
        }
    }
    return root;
}

int main() {
    struct {
        const char* json_str;
        int expect_success; // 1=نتوقع نجاح التحليل، 0=فشل
        const char* test_name;
    } tests[] = {
        {"{\"name\": \"John\", \"age\": 30, \"city\": \"New York\"}", 1, "test_valid_json"},
        {"{}", 1, "test_empty_json"},
        {"[1, 2, 3, 4]", 1, "test_json_with_array"},
        {"{\"person\": {\"name\": \"Alice\", \"age\": 25}, \"city\": \"Wonderland\"}", 1, "test_json_with_nested_objects"},
        {"{\"active\": true, \"verified\": false}", 1, "test_json_with_boolean_values"},
        {"{\"value\": null}", 1, "test_json_with_null_value"},
        {"{\"name\": \"John\", \"age\": 30, \"city\": \"New York\"", 0, "test_malformed_json_missing_bracket"},
        {"{\"name\": \"John\", \"age\": 30,}", 0, "test_malformed_json_unexpected_comma"},
        {"{name: \"John\", \"age\": 30}", 0, "test_malformed_json_unquoted_key"},
        {"Just a string, not JSON", 0, "test_invalid_json_type"}
    };

    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        cJSON* root = getRootElement(tests[i].json_str);
        int success = (root != NULL) ? 1 : 0;

        if (success == tests[i].expect_success) {
            printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
            pass++;
        } else {
            printf("Test %d (%s): FAIL\n", i + 1, tests[i].test_name);
            fail++;
        }

        if (root != NULL) {
            cJSON_Delete(root);
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
