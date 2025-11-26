#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_NUM_WORDS 100

// دالة الترانسبوز من كودك الأصلي مع تعديل طفيف لجمع الناتج في buffer بدل الطباعة مباشرة
int transpose_file_and_capture_output(const char* filename, char* output, size_t max_output_size) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return -1; // خطأ فتح الملف
    }

    char line[MAX_LINE_LENGTH];
    char *words[MAX_NUM_WORDS * MAX_NUM_WORDS];  // مصفوفة مسطحة
    int num_rows = 0;
    int num_cols = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        int i = 0;
        char *token = strtok(line, " \n");
        while (token != NULL) {
            words[num_rows * MAX_NUM_WORDS + i] = strdup(token);
            token = strtok(NULL, " \n");
            i++;
        }
        num_cols = i;
        num_rows++;
    }
    fclose(fp);

    output[0] = '\0';
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < num_rows; j++) {
            strncat(output, words[j * MAX_NUM_WORDS + i], max_output_size - strlen(output) - 1);
            if (j < num_rows - 1)
                strncat(output, " ", max_output_size - strlen(output) - 1);
        }
        strncat(output, "\n", max_output_size - strlen(output) - 1);
    }

    // تحرير الذاكرة
    for (int i = 0; i < num_rows * num_cols; i++) {
        free(words[i]);
    }

    return 0;
}

// دالة مساعدة لإنشاء ملف نصي
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

        int ret = transpose_file_and_capture_output("file.txt", output, sizeof(output));
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
