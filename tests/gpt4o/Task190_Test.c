#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 100

// دالة الترانسبوز من كودك، مع تعديل لجمع الناتج في output بدلاً من الطباعة
int transpose_file(const char* filename, char* output, size_t max_output_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // خطأ فتح الملف
    }
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int num_lines = 0;

    while (fgets(lines[num_lines], MAX_LINE_LENGTH, file)) {
        char *pos;
        if ((pos = strchr(lines[num_lines], '\n')) != NULL) {
            *pos = '\0';
        }
        num_lines++;
        if (num_lines >= MAX_LINES) break;
    }
    fclose(file);

    char *words[MAX_LINES][MAX_LINE_LENGTH];
    int num_words[MAX_LINES] = {0};
    for (int i = 0; i < num_lines; i++) {
        char *token = strtok(lines[i], " ");
        while (token != NULL) {
            words[i][num_words[i]++] = token;
            token = strtok(NULL, " ");
        }
    }

    output[0] = '\0';
    for (int i = 0; i < num_words[0]; i++) {
        for (int j = 0; j < num_lines; j++) {
            strncat(output, words[j][i], max_output_size - strlen(output) - 1);
            if (j < num_lines - 1)
                strncat(output, " ", max_output_size - strlen(output) - 1);
        }
        strncat(output, "\n", max_output_size - strlen(output) - 1);
    }

    return 0;
}

// دالة لإنشاء ملف بمحتوى معين
int create_file(const char* filename, const char* content) {
    FILE *f = fopen(filename, "w");
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

void run_tests() {
    struct {
        const char* input_content;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {
            "name age\nalice 21\nryan 30\n",
            "name alice ryan\nage 21 30\n",
            "Test Case 1"
        },
        {
            "a b c\n1 2 3\nx y z\n",
            "a 1 x\nb 2 y\nc 3 z\n",
            "Test Case 2"
        },
        {
            "foo bar\nbaz qux\n",
            "foo baz\nbar qux\n",
            "Test Case 3"
        },
    };

    char output[10000];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < n; i++) {
        if (create_file("file.txt", tests[i].input_content) != 0) {
            printf("%s: FAIL (Cannot create file)\n", tests[i].description);
            continue;
        }
        output[0] = '\0';

        int ret = transpose_file("file.txt", output, sizeof(output));
        if (ret != 0) {
            printf("%s: FAIL (File open error)\n", tests[i].description);
            continue;
        }

        if (strcmp(output, tests[i].expected_output) == 0) {
            printf("%s: PASS\n", tests[i].description);
        } else {
            printf("%s: FAIL\nExpected:\n%sGot:\n%s\n", tests[i].description, tests[i].expected_output, output);
        }
    }

    remove("file.txt");
}

int main() {
    run_tests();
    return 0;
}
