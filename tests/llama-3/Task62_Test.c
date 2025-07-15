#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parson.h"  // تأكد إن مكتبة parson موجودة ومضمّنة في المشروع

// دالة لتحليل JSON وإرجاع الجذر كنص (للتبسيط سنحول الجذر إلى نص JSON كامل)
char* parse_json_and_get_root(const char* jsonString) {
    JSON_Value *root_value = json_parse_string(jsonString);
    if (root_value == NULL) {
        return NULL;  // خطأ في التحليل
    }

    // سنحول الجذر كامل إلى نص JSON ليتم مقارنته لاحقًا
    char *serialized = json_serialize_to_string(root_value);

    // تحرير ذاكرة الـ JSON_Value لكن نحتفظ بالسلسلة النصية للنتيجة
    json_value_free(root_value);

    // نسخ السلسلة النصية ليتم إرجاعها (يجب تحريرها بعد الاستخدام)
    char *result = strdup(serialized);
    json_free_serialized_string(serialized);

    return result;
}

// دالة مقارنة النتائج وطباعة Pass/Fail
void run_test(const char* test_name, const char* json_input, const char* expected_json, int expect_null) {
    char* root_json = parse_json_and_get_root(json_input);

    int pass = 0;
    if (expect_null) {
        pass = (root_json == NULL);
    } else {
        if (root_json != NULL && strcmp(root_json, expected_json) == 0) {
            pass = 1;
        }
    }

    printf("Test %s: %s\n", test_name, pass ? "PASS" : "FAIL");

    if (root_json) free(root_json);
}

int main() {
    // اختبار كل الحالات حسب بايثون، مع تحويل القيم المتوقعة إلى نص JSON

    run_test("valid_json",
        "{\"name\": \"John\", \"age\": 30, \"city\": \"New York\"}",
        "{\"age\":30,\"city\":\"New York\",\"name\":\"John\"}", 0);

    run_test("empty_json",
        "{}",
        "{}", 0);

    run_test("json_with_array",
        "[1, 2, 3, 4]",
        "[1,2,3,4]", 0);

    run_test("json_with_nested_objects",
        "{\"person\": {\"name\": \"Alice\", \"age\": 25}, \"city\": \"Wonderland\"}",
        "{\"city\":\"Wonderland\",\"person\":{\"age\":25,\"name\":\"Alice\"}}", 0);

    run_test("json_with_boolean_values",
        "{\"active\": true, \"verified\": false}",
        "{\"active\":true,\"verified\":false}", 0);

    run_test("json_with_null_value",
        "{\"value\": null}",
        "{\"value\":null}", 0);

    // اختبارات الخطأ — نتوقع NULL
    run_test("malformed_json_missing_bracket",
        "{\"name\": \"John\", \"age\": 30, \"city\": \"New York\"",
        NULL, 1);

    run_test("malformed_json_unexpected_comma",
        "{\"name\": \"John\", \"age\": 30,}",
        NULL, 1);

    run_test("malformed_json_unquoted_key",
        "{name: \"John\", \"age\": 30}",
        NULL, 1);

    run_test("invalid_json_type",
        "Just a string, not JSON",
        NULL, 1);

    return 0;
}
