#include <stdio.h>
#include <regex.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

// مسار ملف الاختبار (غيّر حسب حاجتك)
const char *test_input_path = "input.txt";

// تنفيذ الكود الأصلي مع استقبال اسم ملف وإخراج النتائج في buffer
int run_regex_match(const char* filepath, char* output_buffer, size_t bufsize) {
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        snprintf(output_buffer, bufsize, "Unable to open file\n");
        return 1;
    }

    regex_t regex;
    int reti = regcomp(&regex, "^[0-9]+\\.$", REG_EXTENDED);
    if (reti) {
        snprintf(output_buffer, bufsize, "Could not compile regex\n");
        fclose(fp);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    output_buffer[0] = '\0';

    while (fgets(line, sizeof(line), fp)) {
        reti = regexec(&regex, line, 0, NULL, 0);
        if (reti == 0) {
            strncat(output_buffer, line, bufsize - strlen(output_buffer) - 1);
        }
    }

    regfree(&regex);
    fclose(fp);
    return 0;
}

// التقاط stdout عند استدعاء الدالة
#include <unistd.h>
void capture_output(int (*func)(const char*, char*, size_t),
                    const char* filepath,
                    char* buffer, size_t bufsize) {
    int pipefd[2];
    pipe(pipefd);

    int saved_fd = dup(1);
    dup2(pipefd[1], 1);
    close(pipefd[1]);

    func(filepath, buffer, bufsize);

    fflush(stdout);

    dup2(saved_fd, 1);
    close(saved_fd);

    ssize_t n = read(pipefd[0], buffer, bufsize - 1);
    if (n >= 0) buffer[n] = '\0';
    else buffer[0] = '\0';
    close(pipefd[0]);
}

// تشغيل التست كيسز
void run_tests() {
    const size_t BUF_SIZE = 65536;
    char output[BUF_SIZE];
    int all_pass = 1;

    printf("Running test cases...\n");

    // Test 1: ملف غير موجود
    printf("Test 1 (non-existent file): ");
    int ret = run_regex_match("no_such_file.txt", output, BUF_SIZE);
    if (ret == 1 && strstr(output, "Unable to open file") != NULL) {
        printf("PASS\n");
    } else {
        printf("FAIL\nOutput:\n%s\n", output);
        all_pass = 0;
    }

    // Test 2: ملف حقيقي ومسار صحيح
    printf("Test 2 (real input file): ");
    capture_output(run_regex_match, test_input_path, output, BUF_SIZE);
    if (strlen(output) > 0) {
        printf("PASS\n");
    } else {
        printf("FAIL - no matching lines found\n");
        all_pass = 0;
    }

    if (all_pass) {
        printf("\nAll tests PASSED.\n");
    } else {
        printf("\nSome tests FAILED.\n");
    }
}

int main(int argc, char **argv) {
    if (argc == 2) {
        char buffer[65536];
        int ret = run_regex_match(argv[1], buffer, sizeof(buffer));
        printf("%s", buffer);
        return ret;
    } else {
        printf("Usage: %s <input_file_path>\n", argv[0]);
        run_tests();
        return 1;
    }
}
