#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>

// دالة لحساب هاش الملف باستخدام خوارزمية محددة (sha256 أو md5)
char* calculate_file_hash(const char* filename, const char* algorithm) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    const EVP_MD *md;
    if (strcmp(algorithm, "sha256") == 0) {
        md = EVP_sha256();
    } else if (strcmp(algorithm, "md5") == 0) {
        md = EVP_md5();
    } else {
        fprintf(stderr, "Unsupported hash algorithm: %s\n", algorithm);
        fclose(fp);
        return NULL;
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, NULL);

    unsigned char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        EVP_DigestUpdate(ctx, buffer, bytes_read);
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    EVP_DigestFinal_ex(ctx, hash, &hash_len);

    EVP_MD_CTX_free(ctx);
    fclose(fp);

    char *hash_string = (char*)malloc(hash_len * 2 + 1);
    for (unsigned int i = 0; i < hash_len; ++i) {
        sprintf(&hash_string[i * 2], "%02x", hash[i]);
    }
    hash_string[hash_len * 2] = '\0';

    return hash_string;
}

// دالة للتحقق من تطابق هاش الملف مع هاش معروف
int verify_file_hash(const char* filename, const char* expected_hash, const char* algorithm) {
    char *calculated_hash = calculate_file_hash(filename, algorithm);
    if (calculated_hash == NULL) {
        return 0; // فشل الحساب
    }

    int result = strcmp(calculated_hash, expected_hash) == 0;
    free(calculated_hash);
    return result;
}

// دالة اختبار صارمة تعرض Pass أو Fail
void run_test(const char* filename, const char* expected_hash, const char* algorithm) {
    int res = verify_file_hash(filename, expected_hash, algorithm);
    if (res) {
        printf("Test verify_file_hash: PASS\n");
    } else {
        printf("Test verify_file_hash: FAIL\n");
    }
}

int main() {
    const char* test_file = "/home/kali/google_cert.pem";

    // هاش شهادة google.com التي زودتني بها (صغيرة كلها حروف صغيرة)
    const char* known_sha256_hash = "bdda50ec92bd745e266b7ba3d157184dfa9328234eb5b5b38e116b16d966fd7e";

    const char* algorithm = "sha256";

    run_test(test_file, known_sha256_hash, algorithm);

    return 0;
}
