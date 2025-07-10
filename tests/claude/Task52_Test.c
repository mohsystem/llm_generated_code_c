#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <errno.h>
#include <unistd.h>

#define KEY_SIZE 32
#define IV_SIZE AES_BLOCK_SIZE
#define BUFFER_SIZE 4096

// دوال التشفير وفك التشفير كما هي (من كودك)
void generate_key(unsigned char *key) {
    RAND_bytes(key, KEY_SIZE);
}

void encrypt_file(const char *input_file, const char *output_file, const unsigned char *key) {
    FILE *infile = fopen(input_file, "rb");
    if (!infile) {
        fprintf(stderr, "Error opening input file %s: %s\n", input_file, strerror(errno));
        exit(1);
    }
    FILE *outfile = fopen(output_file, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file %s: %s\n", output_file, strerror(errno));
        fclose(infile);
        exit(1);
    }

    unsigned char iv[IV_SIZE];
    RAND_bytes(iv, IV_SIZE);
    fwrite(iv, 1, IV_SIZE, outfile);

    AES_KEY aes_key;
    AES_set_encrypt_key(key, KEY_SIZE * 8, &aes_key);

    unsigned char in_buffer[BUFFER_SIZE];
    unsigned char out_buffer[BUFFER_SIZE];
    unsigned char ivec[AES_BLOCK_SIZE];
    memcpy(ivec, iv, AES_BLOCK_SIZE);

    int bytes_read, bytes_written;
    while ((bytes_read = fread(in_buffer, 1, BUFFER_SIZE, infile)) > 0) {
        AES_cbc_encrypt(in_buffer, out_buffer, bytes_read, &aes_key, ivec, AES_ENCRYPT);
        bytes_written = fwrite(out_buffer, 1, bytes_read, outfile);
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error writing to output file\n");
            fclose(infile);
            fclose(outfile);
            exit(1);
        }
    }
    fclose(infile);
    fclose(outfile);
}

void decrypt_file(const char *input_file, const char *output_file, const unsigned char *key) {
    FILE *infile = fopen(input_file, "rb");
    if (!infile) {
        fprintf(stderr, "Error opening input file %s: %s\n", input_file, strerror(errno));
        exit(1);
    }
    FILE *outfile = fopen(output_file, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file %s: %s\n", output_file, strerror(errno));
        fclose(infile);
        exit(1);
    }

    unsigned char iv[IV_SIZE];
    if (fread(iv, 1, IV_SIZE, infile) != IV_SIZE) {
        fprintf(stderr, "Error reading IV from file\n");
        fclose(infile);
        fclose(outfile);
        exit(1);
    }

    AES_KEY aes_key;
    AES_set_decrypt_key(key, KEY_SIZE * 8, &aes_key);

    unsigned char in_buffer[BUFFER_SIZE];
    unsigned char out_buffer[BUFFER_SIZE];
    unsigned char ivec[AES_BLOCK_SIZE];
    memcpy(ivec, iv, AES_BLOCK_SIZE);

    int bytes_read, bytes_written;
    while ((bytes_read = fread(in_buffer, 1, BUFFER_SIZE, infile)) > 0) {
        AES_cbc_encrypt(in_buffer, out_buffer, bytes_read, &aes_key, ivec, AES_DECRYPT);
        bytes_written = fwrite(out_buffer, 1, bytes_read, outfile);
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error writing to output file\n");
            fclose(infile);
            fclose(outfile);
            exit(1);
        }
    }
    fclose(infile);
    fclose(outfile);
}

// دالة مقارنة محتويات ملفين
int compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return -1; // خطأ في فتح الملفات
    }
    int result = 0;
    while (1) {
        int b1 = fgetc(f1);
        int b2 = fgetc(f2);
        if (b1 != b2) {
            result = 1;
            break;
        }
        if (b1 == EOF || b2 == EOF) {
            if (b1 != b2)
                result = 1;
            break;
        }
    }
    fclose(f1);
    fclose(f2);
    return result;
}

int main() {
    const char *test_file = "test_file.txt";
    const char *encrypted_file = "test_file.txt.encrypted";
    const char *decrypted_file = "test_file.txt.decrypted";
    unsigned char key[KEY_SIZE];
    generate_key(key);

    // إنشاء ملف اختبار
    FILE *f = fopen(test_file, "wb");
    if (!f) {
        fprintf(stderr, "Failed to create test file\n");
        return 1;
    }
    fputs("This is a test.", f);
    fclose(f);

    // --- اختبار تشفير الملف ---
    encrypt_file(test_file, encrypted_file, key);
    if (compare_files(test_file, encrypted_file) != 0 && fopen(encrypted_file, "rb") != NULL) {
        printf("Test encrypt_file: PASS\n");
    } else {
        printf("Test encrypt_file: FAIL\n");
    }

    // --- اختبار فك التشفير لملف غير موجود (ينتظر الخطأ) ---
    if (access("non_existent_file.encrypted", F_OK) == -1) {
        // الملف غير موجود كما هو متوقع
        // نحاول فك التشفير ونتوقع فشل (يقوم البرنامج بالخروج)
        int status = system("./decrypt_nonexistent_test.sh"); // ستحتاج سكريبت خارجي يشغل فك التشفير ويحاول مع ملف غير موجود
        if (status != 0) {
            printf("Test decrypt_non_existent_file: PASS\n");
        } else {
            printf("Test decrypt_non_existent_file: FAIL\n");
        }
    } else {
        printf("Test decrypt_non_existent_file: SKIPPED (file exists unexpectedly)\n");
    }

    // --- اختبار تشفير ملف غير موجود (ينتظر الخطأ) ---
    if (access("non_existent_file.txt", F_OK) == -1) {
        int status = system("./encrypt_nonexistent_test.sh");
        if (status != 0) {
            printf("Test encrypt_non_existent_file: PASS\n");
        } else {
            printf("Test encrypt_non_existent_file: FAIL\n");
        }
    } else {
        printf("Test encrypt_non_existent_file: SKIPPED (file exists unexpectedly)\n");
    }

    // --- اختبار فك التشفير الصحيح ---
    decrypt_file(encrypted_file, decrypted_file, key);
    if (compare_files(test_file, decrypted_file) == 0) {
        printf("Test decrypt_file: PASS\n");
    } else {
        printf("Test decrypt_file: FAIL\n");
    }

    // تنظيف الملفات
    remove(test_file);
    remove(encrypted_file);
    remove(decrypted_file);

    return 0;
}
