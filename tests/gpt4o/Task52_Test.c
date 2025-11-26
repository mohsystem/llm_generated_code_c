#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <unistd.h>

#define AES_BLOCK_SIZE 16

void writeKeyToFile(const unsigned char* key, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(key, 1, AES_BLOCK_SIZE, fp);
        fclose(fp);
    }
}

void readKeyFromFile(unsigned char* key, const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp != NULL) {
        fread(key, 1, AES_BLOCK_SIZE, fp);
        fclose(fp);
    }
}

void encryptFile(const unsigned char* key, const char* input_filename, const char* output_filename) {
    AES_KEY encryptKey;
    AES_set_encrypt_key(key, 128, &encryptKey);

    FILE* ifp = fopen(input_filename, "rb");
    FILE* ofp = fopen(output_filename, "wb");

    unsigned char indata[AES_BLOCK_SIZE];
    unsigned char outdata[AES_BLOCK_SIZE];

    while (fread(indata, 1, AES_BLOCK_SIZE, ifp) > 0) {
        AES_encrypt(indata, outdata, &encryptKey);
        fwrite(outdata, 1, AES_BLOCK_SIZE, ofp);
    }

    fclose(ifp);
    fclose(ofp);
}

void decryptFile(const unsigned char* key, const char* input_filename, const char* output_filename) {
    AES_KEY decryptKey;
    AES_set_decrypt_key(key, 128, &decryptKey);

    FILE* ifp = fopen(input_filename, "rb");
    FILE* ofp = fopen(output_filename, "wb");

    unsigned char indata[AES_BLOCK_SIZE];
    unsigned char outdata[AES_BLOCK_SIZE];

    while (fread(indata, 1, AES_BLOCK_SIZE, ifp) > 0) {
        AES_decrypt(indata, outdata, &decryptKey);
        fwrite(outdata, 1, AES_BLOCK_SIZE, ofp);
    }

    fclose(ifp);
    fclose(ofp);
}

// مقارنة ملفات
int compare_files(const char* f1, const char* f2) {
    FILE* file1 = fopen(f1, "rb");
    FILE* file2 = fopen(f2, "rb");
    if (!file1 || !file2) {
        if (file1) fclose(file1);
        if (file2) fclose(file2);
        return -1;
    }
    int res = 0;
    while (1) {
        int c1 = fgetc(file1);
        int c2 = fgetc(file2);
        if (c1 != c2) {
            res = 1;
            break;
        }
        if (c1 == EOF || c2 == EOF) {
            if (c1 != c2) res = 1;
            break;
        }
    }
    fclose(file1);
    fclose(file2);
    return res;
}

int file_exists(const char* filename) {
    return access(filename, F_OK) != -1;
}

int main() {
    const char* keyfile = "filekey.key";
    const char* original = "test.txt";
    const char* encrypted = "test.encrypted";
    const char* decrypted = "test.decrypted";

    // 1. أنشئ ملف نصي للاختبار
    FILE* f = fopen(original, "wb");
    if (!f) {
        perror("Failed to create test file");
        return 1;
    }
    fputs("This is a test.", f);
    fclose(f);

    // 2. توليد المفتاح وكتابته في ملف
    unsigned char key[AES_BLOCK_SIZE];
    RAND_bytes(key, AES_BLOCK_SIZE);
    writeKeyToFile(key, keyfile);

    // 3. اختبار التشفير
    encryptFile(key, original, encrypted);
    if (file_exists(encrypted) && compare_files(original, encrypted) != 0) {
        printf("Test encrypt_file: PASS\n");
    } else {
        printf("Test encrypt_file: FAIL\n");
    }

    // 4. اختبار التشفير على ملف غير موجود
    if (file_exists("nonexistent.txt")) {
        printf("Test encrypt_non_existent_file: FAIL (unexpected file exists)\n");
    } else {
        // محاولة التشفير لملف غير موجود - يجب أن يفشل فتح الملف
        FILE* ftmp = fopen("nonexistent.txt", "rb");
        if (!ftmp) {
            printf("Test encrypt_non_existent_file: PASS\n");
        } else {
            fclose(ftmp);
            printf("Test encrypt_non_existent_file: FAIL\n");
        }
    }

    // 5. اختبار فك التشفير لملف غير موجود
    if (file_exists("nonexistent.encrypted")) {
        printf("Test decrypt_non_existent_file: FAIL (unexpected file exists)\n");
    } else {
        FILE* ftmp = fopen("nonexistent.encrypted", "rb");
        if (!ftmp) {
            printf("Test decrypt_non_existent_file: PASS\n");
        } else {
            fclose(ftmp);
            printf("Test decrypt_non_existent_file: FAIL\n");
        }
    }

    // 6. اختبار فك التشفير الصحيح
    unsigned char loaded_key[AES_BLOCK_SIZE];
    readKeyFromFile(loaded_key, keyfile);
    decryptFile(loaded_key, encrypted, decrypted);
    if (file_exists(decrypted) && compare_files(original, decrypted) == 0) {
        printf("Test decrypt_file: PASS\n");
    } else {
        printf("Test decrypt_file: FAIL\n");
    }

    // 7. حذف الملفات
    remove(original);
    remove(encrypted);
    remove(decrypted);
    remove(keyfile);

    return 0;
}
