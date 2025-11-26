#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <errno.h>

#define AES_BLOCK_SIZE 16

// تعديل دوال التشفير وفك التشفير لترجع كود خطأ بدل إنهاء البرنامج
int encryptFile(const unsigned char* key, const char* inputFile, const char* outputFile) {
    AES_KEY enc_key;
    AES_set_encrypt_key(key, 128, &enc_key);

    FILE* in = fopen(inputFile, "rb");
    if (!in) {
        // perror لطباعة الخطأ
        perror("Error opening input file");
        return -1;
    }
    FILE* out = fopen(outputFile, "wb");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return -2;
    }

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);
    fwrite(iv, 1, AES_BLOCK_SIZE, out);

    unsigned char in_buf[AES_BLOCK_SIZE], out_buf[AES_BLOCK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(in_buf, 1, AES_BLOCK_SIZE, in)) == AES_BLOCK_SIZE) {
        AES_encrypt(in_buf, out_buf, &enc_key);
        if (fwrite(out_buf, 1, AES_BLOCK_SIZE, out) != AES_BLOCK_SIZE) {
            fprintf(stderr, "Error writing to output file\n");
            fclose(in);
            fclose(out);
            return -3;
        }
    }

    if (bytes_read > 0) {
        memset(in_buf + bytes_read, 0, AES_BLOCK_SIZE - bytes_read);
        AES_encrypt(in_buf, out_buf, &enc_key);
        if (fwrite(out_buf, 1, AES_BLOCK_SIZE, out) != AES_BLOCK_SIZE) {
            fprintf(stderr, "Error writing last block to output file\n");
            fclose(in);
            fclose(out);
            return -4;
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}

int decryptFile(const unsigned char* key, const char* inputFile, const char* outputFile) {
    AES_KEY dec_key;
    AES_set_decrypt_key(key, 128, &dec_key);

    FILE* in = fopen(inputFile, "rb");
    if (!in) {
        perror("Error opening input file");
        return -1;
    }
    FILE* out = fopen(outputFile, "wb");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return -2;
    }

    unsigned char iv[AES_BLOCK_SIZE];
    if (fread(iv, 1, AES_BLOCK_SIZE, in) != AES_BLOCK_SIZE) {
        fprintf(stderr, "Error reading IV\n");
        fclose(in);
        fclose(out);
        return -3;
    }

    unsigned char in_buf[AES_BLOCK_SIZE], out_buf[AES_BLOCK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(in_buf, 1, AES_BLOCK_SIZE, in)) == AES_BLOCK_SIZE) {
        AES_decrypt(in_buf, out_buf, &dec_key);
        if (fwrite(out_buf, 1, AES_BLOCK_SIZE, out) != AES_BLOCK_SIZE) {
            fprintf(stderr, "Error writing to output file\n");
            fclose(in);
            fclose(out);
            return -4;
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}

// مقارنة ملفات بايناري
int compare_files(const char* f1, const char* f2) {
    FILE* file1 = fopen(f1, "rb");
    FILE* file2 = fopen(f2, "rb");
    if (!file1 || !file2) {
        if (file1) fclose(file1);
        if (file2) fclose(file2);
        return -1;
    }
    int result = 0;
    while (1) {
        int c1 = fgetc(file1);
        int c2 = fgetc(file2);
        if (c1 != c2) {
            result = 1;
            break;
        }
        if (c1 == EOF || c2 == EOF) {
            if (c1 != c2) result = 1;
            break;
        }
    }
    fclose(file1);
    fclose(file2);
    return result;
}

int main() {
    const char* original = "test_file.txt";
    const char* encrypted = "test_file.txt.encrypted";
    const char* decrypted = "test_file.txt.decrypted";

    unsigned char key[16];
    RAND_bytes(key, 16);

    // إنشاء ملف نصي للاختبار
    FILE* f = fopen(original, "wb");
    if (!f) {
        perror("Create test file");
        return 1;
    }
    fputs("This is a test.", f);
    fclose(f);

    // اختبار تشفير الملف
    int enc_res = encryptFile(key, original, encrypted);
    if (enc_res == 0) {
        // تحقق أن الملف المشفر موجود ومختلف عن الأصلي
        if (compare_files(original, encrypted) != 0) {
            printf("Test encrypt_file: PASS\n");
        } else {
            printf("Test encrypt_file: FAIL (encrypted file same as original)\n");
        }
    } else {
        printf("Test encrypt_file: FAIL (encryption error code %d)\n", enc_res);
    }

    // اختبار تشفير ملف غير موجود
    int enc_nonexistent = encryptFile(key, "non_existent_file.txt", encrypted);
    if (enc_nonexistent != 0) {
        printf("Test encrypt_non_existent_file: PASS\n");
    } else {
        printf("Test encrypt_non_existent_file: FAIL\n");
    }

    // اختبار فك تشفير ملف غير موجود
    int dec_nonexistent = decryptFile(key, "non_existent_file.encrypted", decrypted);
    if (dec_nonexistent != 0) {
        printf("Test decrypt_non_existent_file: PASS\n");
    } else {
        printf("Test decrypt_non_existent_file: FAIL\n");
    }

    // اختبار فك التشفير الصحيح
    int dec_res = decryptFile(key, encrypted, decrypted);
    if (dec_res == 0) {
        if (compare_files(original, decrypted) == 0) {
            printf("Test decrypt_file: PASS\n");
        } else {
            printf("Test decrypt_file: FAIL (decrypted file differs from original)\n");
        }
    } else {
        printf("Test decrypt_file: FAIL (decryption error code %d)\n", dec_res);
    }

    // تنظيف الملفات
    remove(original);
    remove(encrypted);
    remove(decrypted);

    return 0;
}
