#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

#define MAX_INPUT_SIZE 1024

// الكود الأصلي محفوظ كما هو (main)

// === Test framework ===
#define ASSERT(cond, testname) do { \
if (cond) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// === Test cases ===

void test_valid_json_object() {
    const char* valid_json = "{\"name\": \"John\", \"age\": 30}";
    cJSON *json = cJSON_Parse(valid_json);
    ASSERT(json != NULL, "test_valid_json_object");
    if (json) cJSON_Delete(json);
}

void test_valid_json_array() {
    const char* valid_json = "[1, 2, 3, 4]";
    cJSON *json = cJSON_Parse(valid_json);
    ASSERT(json != NULL, "test_valid_json_array");
    if (json) cJSON_Delete(json);
}

void test_invalid_json_syntax() {
    const char* invalid_json = "{\"name\": \"John\", \"age\": 30"; // missing closing }
    cJSON *json = cJSON_Parse(invalid_json);
    ASSERT(json == NULL, "test_invalid_json_syntax");
}

void test_empty_string() {
    const char* empty_str = "";
    cJSON *json = cJSON_Parse(empty_str);
    ASSERT(json == NULL, "test_empty_string");
}

void test_non_json_string() {
    const char* non_json = "Just a random string";
    cJSON *json = cJSON_Parse(non_json);
    ASSERT(json == NULL, "test_non_json_string");
}

// صعب اختبار الحالات المعقدة مثل تعطل الذاكرة أو أخطاء غير متوقعة، نطبع fail
void test_unexpected_error() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_unexpected_error");
}

int main() {
    test_valid_json_object();
    test_valid_json_array();
    test_invalid_json_syntax();
    test_empty_string();
    test_non_json_string();
    test_unexpected_error();
    return 0;
}
