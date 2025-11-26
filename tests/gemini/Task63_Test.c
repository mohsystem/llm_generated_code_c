#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parson.h" // تحتاج تضمين مكتبة parson في مشروعك

// دالة بسيطة لفك Base64 (مقتبسة مع تبسيط)
static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
unsigned char *base64_decode(const char *data, size_t *out_len) {
    if (data == NULL) return NULL;
    size_t len = strlen(data);
    if (len % 4 != 0) return NULL;

    size_t padding = 0;
    if (len >= 1 && data[len-1] == '=') padding++;
    if (len >= 2 && data[len-2] == '=') padding++;

    *out_len = (len / 4) * 3 - padding;
    unsigned char *decoded = malloc(*out_len + 1);
    if (!decoded) return NULL;

    int decode_table[256];
    for (int i=0; i<256; i++) decode_table[i] = -1;
    for (int i=0; i<64; i++) decode_table[(unsigned char)b64_table[i]] = i;

    size_t j=0;
    for (size_t i=0; i<len; i+=4) {
        int n = decode_table[(unsigned char)data[i]] << 18 |
                decode_table[(unsigned char)data[i+1]] << 12 |
                decode_table[(unsigned char)data[i+2]] << 6  |
                decode_table[(unsigned char)data[i+3]];
        decoded[j++] = (n >> 16) & 0xFF;
        if (data[i+2] != '=') decoded[j++] = (n >> 8) & 0xFF;
        if (data[i+3] != '=') decoded[j++] = n & 0xFF;
    }
    decoded[*out_len] = '\0';
    return decoded;
}

// الدالة المطلوب اختبارها
JSON_Value* process_request(const char *raw_data) {
    if (raw_data == NULL || strlen(raw_data) == 0) return NULL;

    size_t decoded_len = 0;
    unsigned char *decoded_data = base64_decode(raw_data, &decoded_len);
    if (decoded_data == NULL) return NULL;

    // null terminate
    decoded_data[decoded_len] = '\0';

    JSON_Value *json_value = json_parse_string((const char*)decoded_data);
    free(decoded_data);
    return json_value; // caller must free
}

// دالة مساعدة للمقارنة وطباعة Pass/Fail
void run_test(const char* test_name, const char* raw_data, const char* expected_json_str, int expect_null) {
    JSON_Value *value = process_request(raw_data);
    int pass = 0;
    if (expect_null) {
        pass = (value == NULL);
    } else {
        if (value != NULL) {
            char *serialized = json_serialize_to_string(value);
            pass = (serialized != NULL && strcmp(serialized, expected_json_str) == 0);
            json_free_serialized_string(serialized);
        }
    }

    printf("Test %s: %s\n", test_name, pass ? "PASS" : "FAIL");
    if (value) json_value_free(value);
}

int main() {
    // Note: parson serializes JSON objects with keys sorted lexically,
    // so adjust expected JSON strings accordingly.

    run_test("valid_json",
             "eyJuYW1lIjogIkpvaG4iLCAiYWdlIjogMzAsICJjaXR5IjogIk5ldyBZb3JrIn0=",
             "{\"age\":30,\"city\":\"New York\",\"name\":\"John\"}", 0);

    run_test("empty_input", "", NULL, 1);

    run_test("invalid_base64", "invalid_base64_string", NULL, 1);

    // malformed json: base64 of 'name": "John}'
    run_test("malformed_json",
             "bmFtZSI6ICJKb2hufQ==", // base64 of malformed string
             NULL, 1);

    run_test("base64_of_empty_json",
             "e30=",
             "{}", 0);

    run_test("json_with_special_characters",
             "eyJrZXkiOiAidmFsdWVfd2l0aF9zcGVjaWFsX2NoYXJhY3RlcnNfIUAjJCVeJiooKSJ9",
             "{\"key\":\"value_with_special_characters_!@#$%^&*()\"}", 0);

    run_test("json_with_numeric_values",
             "eyJ2YWx1ZSI6IDEyMzR9",
             "{\"value\":1234}", 0);

    run_test("json_with_boolean_values",
             "eyJhY3RpdmUiOiB0cnVlLCAidmVyaWZpZWQiOiBmYWxzZX0=",
             "{\"active\":true,\"verified\":false}", 0);

    run_test("json_with_null_value",
             "eyJ2YWx1ZSI6IG51bGx9",
             "{\"value\":null}", 0);

    run_test("nested_json",
             "eyJvdXRlciI6IHsiaW5uZXIiOiAidmFsdWUifX0=",
             "{\"outer\":{\"inner\":\"value\"}}", 0);

    return 0;
}
