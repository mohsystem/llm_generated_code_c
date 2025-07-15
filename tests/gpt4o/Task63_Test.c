#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

// --- دالة base64_decode مضمّنة ---

static const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int b64index[256] = {0};

void build_b64index() {
    static int built = 0;
    if (built) return;
    for (int i = 0; i < 256; i++) b64index[i] = -1;
    for (int i = 0; i < 64; i++) b64index[(unsigned char)b64chars[i]] = i;
    built = 1;
}

unsigned char *base64_decode_simple(const char *in, size_t len, size_t *out_len) {
    build_b64index();
    if (len % 4 != 0) return NULL;

    size_t padding = 0;
    if (len >= 1 && in[len-1] == '=') padding++;
    if (len >= 2 && in[len-2] == '=') padding++;

    *out_len = (len / 4) * 3 - padding;
    unsigned char *out = malloc(*out_len + 1);
    if (!out) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i += 4) {
        int n = (b64index[(unsigned char)in[i]] << 18) |
                (b64index[(unsigned char)in[i+1]] << 12) |
                (b64index[(unsigned char)in[i+2]] << 6) |
                (b64index[(unsigned char)in[i+3]]);

        out[j++] = (n >> 16) & 0xFF;
        if (in[i+2] != '=') out[j++] = (n >> 8) & 0xFF;
        if (in[i+3] != '=') out[j++] = n & 0xFF;
    }
    out[*out_len] = '\0';
    return out;
}

// --- دالة معالجة الطلب ---

char* process_request(const char* req) {
    if (!req || strlen(req) == 0) {
        return NULL;
    }
    size_t outlen = 0;
    unsigned char *decoded_data = base64_decode_simple(req, strlen(req), &outlen);
    if (!decoded_data) return NULL;

    decoded_data[outlen] = '\0';

    json_error_t error;
    json_t *deserialized_data = json_loads((const char*)decoded_data, 0, &error);
    free(decoded_data);

    if (!deserialized_data) {
        return NULL;
    }

    char *result = json_dumps(deserialized_data, JSON_COMPACT);
    json_decref(deserialized_data);
    return result;
}

// --- دالة اختبار كل حالة ---

void run_test(const char* test_name, const char* raw_data_base64, const char* expected_json_compact, int expect_null) {
    char* result = process_request(raw_data_base64);
    int pass = 0;

    if (expect_null) {
        pass = (result == NULL);
    } else {
        if (result != NULL && strcmp(result, expected_json_compact) == 0) {
            pass = 1;
        }
    }

    printf("Test %s: %s\n", test_name, pass ? "PASS" : "FAIL");

    if (result) free(result);
}

// --- الدالة الرئيسية ---

int main() {
    run_test("valid_json",
             "eyJuYW1lIjogIkpvaG4gRG9lIiwgImFnZSI6IDMwfQ==",
             "{\"age\":30,\"name\":\"John Doe\"}", 0);

    run_test("empty_input",
             "",
             NULL, 1);

    run_test("invalid_base64",
             "invalid_base64_string",
             NULL, 1);

    run_test("malformed_json",
             "bmFtZSI6ICJKb2hufQ==", // base64 of malformed JSON fragment 'name": "John}'
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
