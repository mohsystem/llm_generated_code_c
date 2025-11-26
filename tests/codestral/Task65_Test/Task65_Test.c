#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for pipe, dup2, read, close

// دالة اختبار اعادة توجيه الإخراج
int capture_output(void (*func)(int, char**), int argc, char* argv[], char* output_buf, size_t buf_size) {
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;

    fflush(stdout);
    int stdout_copy = dup(fileno(stdout));
    if (stdout_copy == -1) return -1;

    dup2(pipefd[1], fileno(stdout));
    close(pipefd[1]);

    func(argc, argv);

    fflush(stdout);
    dup2(stdout_copy, fileno(stdout));
    close(stdout_copy);

    ssize_t len = read(pipefd[0], output_buf, buf_size - 1);
    close(pipefd[0]);

    if (len < 0) return -1;
    output_buf[len] = '\0';
    return 0;
}

// إعادة كتابة main كدالة منفصلة لاختبارها
void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./FileReader <filename>\n");
        return;
    }

    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        printf("Error: Unable to open the file.\n");
    }
}

// طباعة النتائج
void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

// اختبار argc != 2
void test_wrong_argc() {
    char output[512];
    char* argv[] = { "program" };
    int argc = 1;
    if (capture_output(program_main, argc, argv, output, sizeof(output)) != 0) {
        print_result("test_wrong_argc", 0);
        return;
    }
    if (strstr(output, "Usage: ./FileReader <filename>") != NULL)
        print_result("test_wrong_argc", 1);
    else
        print_result("test_wrong_argc", 0);
}

// اختبار فتح ملف موجود (استخدم مسارك للملف)
// تأكد من وجود الملف قبل التشغيل
void test_file_exists() {
    const char* filepath = "/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task65_Test/testFile.txt";
    char output[8192];
    char* argv[] = { "program", (char*)filepath };
    int argc = 2;
    if (capture_output(program_main, argc, argv, output, sizeof(output)) != 0) {
        print_result("test_file_exists", 0);
        return;
    }
    if (strstr(output, "Error: Unable to open the file.") == NULL && strlen(output) > 0)
        print_result("test_file_exists", 1);
    else
        print_result("test_file_exists", 0);
}

// اختبار ملف غير موجود
void test_file_not_exist() {
    char output[512];
    char* argv[] = { "program", "nonexistentfile.txt" };
    int argc = 2;
    if (capture_output(program_main, argc, argv, output, sizeof(output)) != 0) {
        print_result("test_file_not_exist", 0);
        return;
    }
    if (strstr(output, "Error: Unable to open the file.") != NULL)
        print_result("test_file_not_exist", 1);
    else
        print_result("test_file_not_exist", 0);
}

// اختبار اسم ملف غير صحيح (لا يوجد تحقق في الكود الحالي لذلك نطبع fail not implemented)
void test_invalid_filename() {
    print_result("test_invalid_filename", 0);
}

int main() {
    test_wrong_argc();
    test_file_exists();
    test_file_not_exist();
    test_invalid_filename();  // غير مغطى بالكود الحالي
    return 0;
}
