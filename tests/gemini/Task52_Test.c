#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <errno.h>
#include <unistd.h>  // for access()

#define AES_BLOCK_SIZE 16

void generate_key(unsigned char* key) {
    RAND_bytes(key, AES_BLOCK_SIZE);
}

int encrypt_file(const char* input_file, const char* output_file, unsigned char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return 0;

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    FILE* in_file = fopen(input_file, "rb");
    FILE* out_file = fopen(output_file, "wb");
    if (!in_file || !out_file) {
        if (in_file) fclose(in_file);
        if (out_file) fclose(out_file);
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    const size_t buffer_size = 1024;
    unsigned char in_buffer[buffer_size];
    unsigned char out_buffer[buffer_size + AES_BLOCK_SIZE];
    int out_len;
    size_t read_len;
    int total_len = 0;

    while ((read_len = fread(in_buffer, 1, buffer_size, in_file)) > 0) {
        if (EVP_EncryptUpdate(ctx, out_buffer, &out_len, in_buffer, (int)read_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            fclose(in_file);
            fclose(out_file);
            return 0;
        }
        fwrite(out_buffer, 1, out_len, out_file);
        total_len += out_len;
    }

    if (EVP_EncryptFinal_ex(ctx, out_buffer, &out_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        fclose(in_file);
        fclose(out_file);
        return 0;
    }
    fwrite(out_buffer, 1, out_len, out_file);
    total_len += out_len;

    EVP_CIPHER_CTX_free(ctx);
    fclose(in_file);
    fclose(out_file);
    return total_len;
}

int decrypt_file(const char* input_file, const char* output_file, unsigned char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return 0;

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    FILE* in_file = fopen(input_file, "rb");
    FILE* out_file = fopen(output_file, "wb");
    if (!in_file || !out_file) {
        if (in_file) fclose(in_file);
        if (out_file) fclose(out_file);
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    const size_t buffer_size = 1024;
    unsigned char in_buffer[buffer_size];
    unsigned char out_buffer[buffer_size + AES_BLOCK_SIZE];
    int out_len;
    size_t read_len;
    int total_len = 0;

    while ((read_len = fread(in_buffer, 1, buffer_size, in_file)) > 0) {
        if (EVP_DecryptUpdate(ctx, out_buffer, &out_len, in_buffer, (int)read_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            fclose(in_file);
            fclose(out_file);
            return 0;
        }
        fwrite(out_buffer, 1, out_len, out_file);
        total_len += out_len;
    }

    if (EVP_DecryptFinal_ex(ctx, out_buffer, &out_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        fclose(in_file);
        fclose(out_file);
        return 0;
    }
    fwrite(out_buffer, 1, out_len, out_file);
    total_len += out_len;

    EVP_CIPHER_CTX_free(ctx);
    fclose(in_file);
    fclose(out_file);
    return total_len;
}

int compare_files(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "rb");
    FILE* f2 = fopen(file2, "rb");
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return -1;
    }
    int diff = 0;
    while (1) {
        int c1 = fgetc(f1);
        int c2 = fgetc(f2);
        if (c1 != c2) {
            diff = 1;
            break;
        }
        if (c1 == EOF || c2 == EOF) break;
    }
    fclose(f1);
    fclose(f2);
    return diff;
}

int main() {
    unsigned char key[AES_BLOCK_SIZE];
    generate_key(key);

    const char* original = "test.txt";
    const char* encrypted = "test.txt.encrypted";
    const char* decrypted = "test.txt.decrypted";

    // إنشاء ملف نصي للاختبار
    FILE* f = fopen(original, "wb");
    if (!f) {
        perror("Failed to create test file");
        return 1;
    }
    fputs("This is a test.", f);
    fclose(f);

    // 1. اختبار تشفير الملف
    if (encrypt_file(original, encrypted, key) > 0) {
        if (compare_files(original, encrypted) != 0) {
            printf("Test encrypt_file: PASS\n");
        } else {
            printf("Test encrypt_file: FAIL (encrypted same as original)\n");
        }
    } else {
        printf("Test encrypt_file: FAIL (encryption failed)\n");
    }

    // 2. اختبار تشفير ملف غير موجود
    if (encrypt_file("non_existent_file.txt", encrypted, key) == 0) {
        printf("Test encrypt_non_existent_file: FAIL\n");
    } else {
        printf("Test encrypt_non_existent_file: PASS\n");
    }

    // 3. اختبار فك تشفير ملف غير موجود
    if (decrypt_file("non_existent_file.encrypted", decrypted, key) == 0) {
        printf("Test decrypt_non_existent_file: FAIL\n");
    } else {
        printf("Test decrypt_non_existent_file: PASS\n");
    }

    // 4. اختبار فك التشفير الصحيح
    if (decrypt_file(encrypted, decrypted, key) > 0 && compare_files(original, decrypted) == 0) {
        printf("Test decrypt_file: PASS\n");
    } else {
        printf("Test decrypt_file: FAIL\n");
    }

    // تنظيف الملفات
    remove(original);
    remove(encrypted);
    remove(decrypted);

    return 0;
}
