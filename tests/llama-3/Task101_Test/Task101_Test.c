#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const char *filename = "script.sh";
    const char *expected_content = "#!/bin/bash\necho 'Hello World!\n"; // كما هو مكتوب في الكود (ملاحظة الاقتباس غير مغلق)

    // إنشاء الملف
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Test result: FAIL (file open)\n");
        return 1;
    }

    fprintf(file, "#!/bin/bash\necho 'Hello World!\n"); // نفس نص الكود (غير مكتمل)
    fclose(file);

    // تغيير صلاحيات الملف
    if (system("chmod +x script.sh") != 0) {
        printf("Test result: FAIL (chmod)\n");
        return 1;
    }

    // قراءة الملف للتحقق من المحتوى
    file = fopen(filename, "r");
    if (!file) {
        printf("Test result: FAIL (file open for verification)\n");
        return 1;
    }

    char buffer[512];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[len] = '\0';
    fclose(file);

    if (strcmp(buffer, expected_content) != 0) {
        printf("Test result: FAIL (content mismatch)\n");
        return 1;
    }

    // التحقق من صلاحيات الملف
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("stat");
        printf("Test result: FAIL (stat)\n");
        return 1;
    }

    mode_t expected_mode = S_IRWXU; // 755
    if ((st.st_mode & 0777) != expected_mode) {
        printf("Test result: FAIL (permissions mismatch)\n");
        return 1;
    }

    printf("Test result: PASS\n");
    return 0;
}
