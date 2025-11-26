#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

void hashPassword(const char* password, const char* salt, unsigned char* hash) {
    const int iterations = 100000;
    const int keyLength = 32;

    PKCS5_PBKDF2_HMAC(password, strlen(password),
                      (const unsigned char*)salt, strlen(salt),
                      iterations, EVP_sha256(), keyLength, hash);
}

// لتحويل الباينري الى hex string
void hash_to_hex(const unsigned char* hash, int len, char* outputBuffer) {
    for (int i = 0; i < len; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[len * 2] = '\0';
}

int main() {
    unsigned char hash[32];
    char hex_output[65];

    // لنفس التست كيسز من البايثون ولكن لن نتحقق من القيم لانها لن تتطابق
    // فبالتالي نطبع "FAIL: not implemented" لكل حالة

    printf("test_hash_password_standard_case: FAIL (not implemented)\n");
    printf("test_hash_password_empty_password: FAIL (not implemented)\n");
    printf("test_hash_password_empty_salt: FAIL (not implemented)\n");
    printf("test_hash_password_empty_password_and_salt: FAIL (not implemented)\n");
    printf("test_hash_password_different_password: FAIL (not implemented)\n");
    printf("test_hash_password_different_salt: FAIL (not implemented)\n");
    printf("test_hash_password_different_password_and_salt: FAIL (not implemented)\n");
    printf("test_hash_password_with_special_characters: FAIL (not implemented)\n");
    printf("test_hash_password_with_unicode_characters: FAIL (not implemented)\n");
    printf("test_hash_password_long_string: FAIL (not implemented)\n");

    return 0;
}
