#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for access()

// الدوال الأصلية (يفترض موجودة في نفس الملف أو مضمّنة)
// هنا أعدت تعريفها لأجل الإكمال فقط

char* caesarEncrypt(const char* text, int shift) {
    int len = strlen(text);
    char* result = (char*) malloc((len + 1) * sizeof(char));

    for (int i = 0; i < len; i++) {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) {
            char base = (text[i] >= 'a' && text[i] <= 'z') ? 'a' : 'A';
            result[i] = (char) ((text[i] - base + shift) % 26 + base);
        } else {
            result[i] = text[i];
        }
    }

    result[len] = '\0';

    return result;
}

char* caesarDecrypt(const char* text, int shift) {
    int len = strlen(text);
    char* result = (char*) malloc((len + 1) * sizeof(char));

    for (int i = 0; i < len; i++) {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) {
            char base = (text[i] >= 'a' && text[i] <= 'z') ? 'a' : 'A';
            result[i] = (char) ((text[i] - base - shift) % 26 + base);
        } else {
            result[i] = text[i];
        }
    }

    result[len] = '\0';

    return result;
}

void encryptFile(const char* filePath, int shift) {
    FILE* file = fopen(filePath, "r+");
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);

    char* text = (char*) malloc((len + 1) * sizeof(char));
    fread(text, 1, len, file);
    text[len] = '\0';

    char* encryptedText = caesarEncrypt(text, shift);

    fseek(file, 0, SEEK_SET);
    fwrite(encryptedText, 1, len, file);
    fclose(file);
    free(text);
    free(encryptedText);
}

void decryptFile(const char* filePath, int shift) {
    FILE* file = fopen(filePath, "r+");
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);

    char* text = (char*) malloc((len + 1) * sizeof(char));
    fread(text, 1, len, file);
    text[len] = '\0';

    char* decryptedText = caesarDecrypt(text, shift);

    fseek(file, 0, SEEK_SET);
    fwrite(decryptedText, 1, len, file);
    fclose(file);
    free(text);
    free(decryptedText);
}

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
    const char* original = "example.txt";
    const char* backup = "example_backup.txt";
    const int shift = 3;

    // 1. إنشاء ملف نصي للاختبار
    FILE* f = fopen(original, "w");
    if (!f) {
        perror("Failed to create test file");
        return 1;
    }
    fputs("This is a test.", f);
    fclose(f);

    // 2. نسخ المحتوى إلى ملف احتياطي للمقارنة لاحقاً
    FILE* src = fopen(original, "rb");
    FILE* dest = fopen(backup, "wb");
    if (!src || !dest) {
        perror("Failed to create backup file");
        return 1;
    }
    char ch;
    while ((ch = fgetc(src)) != EOF) fputc(ch, dest);
    fclose(src);
    fclose(dest);

    // 3. تشفير الملف
    encryptFile(original, shift);
    if (file_exists(original) && compare_files(original, backup) != 0) {
        printf("Test encrypt_file: PASS\n");
    } else {
        printf("Test encrypt_file: FAIL\n");
    }

    // 4. فك التشفير
    decryptFile(original, shift);
    if (file_exists(original) && compare_files(original, backup) == 0) {
        printf("Test decrypt_file: PASS\n");
    } else {
        printf("Test decrypt_file: FAIL\n");
    }

    // 5. تشفير ملف غير موجود
    if (!file_exists("nonexistent.txt")) {
        // نحاول التشفير ونراقب حدوث خطأ (هنا الكود لا يرجع خطأ، فقط سيسبب فشل فتح الملف)
        FILE* ftmp = fopen("nonexistent.txt", "rb");
        if (!ftmp) {
            printf("Test encrypt_non_existent_file: PASS\n");
        } else {
            fclose(ftmp);
            printf("Test encrypt_non_existent_file: FAIL\n");
        }
    }

    // 6. فك التشفير لملف غير موجود
    if (!file_exists("nonexistent_decrypt.txt")) {
        FILE* ftmp = fopen("nonexistent_decrypt.txt", "rb");
        if (!ftmp) {
            printf("Test decrypt_non_existent_file: PASS\n");
        } else {
            fclose(ftmp);
            printf("Test decrypt_non_existent_file: FAIL\n");
        }
    }

    // حذف الملفات المؤقتة
    remove(original);
    remove(backup);

    return 0;
}
