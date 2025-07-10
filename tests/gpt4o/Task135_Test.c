#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

typedef struct {
    char token[SHA256_DIGEST_LENGTH*2 + 1 + 50]; // زيادة مساحة ل sessionId المضاف
} CSRFProtection;

void generateToken(CSRFProtection* csrf, const char* sessionId) {
    unsigned char randomValue[60];
    FILE* urandom = fopen("/dev/urandom", "rb");
    fread(randomValue, sizeof(randomValue), 1, urandom);
    fclose(urandom);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(randomValue, sizeof(randomValue), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(csrf->token + (i * 2), "%02x", hash[i]);
    }
    csrf->token[SHA256_DIGEST_LENGTH*2] = '\0'; // تأكد من نهاية النص
    strcat(csrf->token, sessionId);
}

int validateToken(CSRFProtection* csrf, const char* token, const char* sessionId) {
    char expectedToken[sizeof(csrf->token)];
    strcpy(expectedToken, csrf->token);
    strcat(expectedToken, sessionId);
    return strcmp(expectedToken, token) == 0;
}

void test_case(const char* sessionId, const char* testToken, int expected) {
    CSRFProtection csrf;
    generateToken(&csrf, sessionId);

    int valid = validateToken(&csrf, testToken, sessionId);
    printf("Test with sessionId='%s' token='%s': %s\n",
           sessionId,
           testToken ? testToken : "NULL",
           (valid == expected) ? "PASS" : "FAIL");
}

int main() {
    const char* sessionId = "12345ABC";

    CSRFProtection csrf;
    generateToken(&csrf, sessionId);
    printf("Generated Token: %s\n\n", csrf.token);

    // حالات الاختبار:
    // 1. اختبار التوكن الصحيح (يجب ينجح)
    test_case(sessionId, csrf.token, 1);

    // 2. اختبار توكن غير صحيح (يجب يفشل)
    test_case(sessionId, "wrongtoken123", 0);

    // 3. اختبار NULL توكن (يجب يفشل)
    test_case(sessionId, NULL, 0);

    // 4. اختبار توكن صحيح مع sessionId مختلف (يجب يفشل)
    char token_with_diff_session[sizeof(csrf.token)];
    strcpy(token_with_diff_session, csrf.token);
    strcat(token_with_diff_session, "DIFFERENT_SESSION");
    test_case("DIFFERENT_SESSION", token_with_diff_session, 0);

    return 0;
}
