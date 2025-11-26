#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int main() {
    const char* filename = "script.sh";
    const char* expected_content = "#!/bin/bash \\n echo \"Hello, World!\"";

    // إنشاء الملف
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fputs(expected_content, file);
        fclose(file);
        printf("File created successfully.\n");
    } else {
        fprintf(stderr, "Unable to create file.\n");
        printf("Test result: FAIL (file creation)\n");
        return 1;
    }

    // جعل الملف قابل للتنفيذ
    if (chmod(filename, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0) {
        printf("File made executable successfully.\n");
    } else {
        fprintf(stderr, "Unable to change file permissions.\n");
        printf("Test result: FAIL (chmod)\n");
        return 1;
    }

    // التحقق من الملف - قراءة المحتوى ومقارنته
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file for verification.\n");
        printf("Test result: FAIL (verification - open file)\n");
        return 1;
    }

    // قراءة محتوى الملف
    char buffer[256];
    size_t read_len = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[read_len] = '\0';
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

    // صلاحيات التنفيذ للمالك، المجموعة، والآخرين (755)
    mode_t expected_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    if ((st.st_mode & 0777) != expected_mode) {
        printf("Test result: FAIL (permissions mismatch)\n");
        return 1;
    }

    // كل شيء صحيح
    printf("Test result: PASS\n");
    return 0;
}
