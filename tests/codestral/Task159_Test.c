#include <stdio.h>
#include <stdlib.h>

int runTest(int index) {
    FILE *file = fopen("buffer.dat", "rb");
    if (!file) {
        printf("Failed to open buffer file.\n");
        return 0;
    }

    // تحقق من صلاحية المؤشر
    if (index < 0 || index >= 1024) {
        printf("Invalid index %d. Must be between 0 and 1023.\n", index);
        fclose(file);
        return 1;  // نعتبره ناجحًا إذا تم التعامل مع الخطأ بشكل صحيح
    }

    fseek(file, index, SEEK_SET);
    char value;
    fread(&value, sizeof(value), 1, file);
    fclose(file);

    printf("Value at index %d: %d\n", index, (int)value);
    return 1; // نجاح الاختبار
}

int main() {
    // أنشئ ملف buffer.dat مع 1024 بايت كلها 0
    FILE *file = fopen("buffer.dat", "wb");
    if (!file) {
        printf("Failed to create buffer file.\n");
        return 1;
    }
    char zero = 0;
    for (int i = 0; i < 1024; i++) {
        fwrite(&zero, sizeof(zero), 1, file);
    }
    fclose(file);

    // التيست كيسز (مثل البايثون الأصلية)
    int testIndices[] = {0, 10, 25, 1023, 1024, -1, -5, 512};
    int totalTests = sizeof(testIndices) / sizeof(testIndices[0]);
    int passed = 0;

    for (int i = 0; i < totalTests; i++) {
        printf("Test case %d: index = %d\n", i + 1, testIndices[i]);
        passed += runTest(testIndices[i]);
    }

    printf("\nPassed %d/%d test cases.\n", passed, totalTests);
    return 0;
}
