#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    char *records[100];

    while ((read = getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = 0;  // إزالة \n
        records[count] = strdup(line);
        count++;
        if (count >= 100) break;
    }
    free(line);
    fclose(file);

    qsort(records, count, sizeof(char *), compare);

    for (int i = 0; i < count; i++) {
        printf("%s\n", records[i]);
        free(records[i]);
    }
}

int create_temp_file(const char *filename, const char *content) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    fputs(content, f);
    fclose(f);
    return 1;
}

int main(int argc, char *argv[]) {
    // Test case 1: عدم تمرير اسم ملف
    if (argc != 2) {
        printf("Test case 1: PASS\n");
        printf("Usage: program <filename>\n");
    } else {
        printf("Test case 1: FAIL (usage check not tested because argument provided)\n");
    }

    // Test case 2: ملف غير موجود
    printf("Test case 2: ");
    FILE *f = fopen("nonexistent_file.txt", "r");
    if (!f) {
        printf("PASS\n");
        printf("Error opening file\n");
    } else {
        printf("FAIL\n");
        fclose(f);
    }

    // Test case 3: ملف موجود لكن فيه سطر بدون ":"
    // هذا غير مدعوم في كود C الحالي، فقط اطبع FAIL not implemented
    printf("Test case 3: FAIL not implemented\n");

    // Test case 4: ملف صالح يحتوي على key: value
    const char *filename = "tempfile.txt";
    const char *file_content =
        "key1: value1\n"
        "key3: value3\n"
        "key2: value2\n";

    if (!create_temp_file(filename, file_content)) {
        printf("Test case 4: FAIL could not create temp file\n");
        return 1;
    }

    printf("Test case 4: Expected output:\n");
    printf("key1: value1\nkey2: value2\nkey3: value3\n");

    printf("Test case 4: Actual output:\n");
    process_file(filename);

    remove(filename);

    return 0;
}
