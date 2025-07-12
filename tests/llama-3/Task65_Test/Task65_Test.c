#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // pipe, dup2, read, close

// نعيد كتابة main كدالة منفصلة للاختبار
void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return;
    }
    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

// دالة لالتقاط stdout و stderr
int capture_output(void (*func)(int,char**), int argc, char* argv[],
                   char* out_buf, size_t out_size,
                   char* err_buf, size_t err_size) {
    int out_pipe[2], err_pipe[2];
    if (pipe(out_pipe) == -1) return -1;
    if (pipe(err_pipe) == -1) return -1;

    fflush(stdout);
    fflush(stderr);

    int stdout_copy = dup(fileno(stdout));
    int stderr_copy = dup(fileno(stderr));
    if (stdout_copy == -1 || stderr_copy == -1) return -1;

    dup2(out_pipe[1], fileno(stdout));
    dup2(err_pipe[1], fileno(stderr));
    close(out_pipe[1]);
    close(err_pipe[1]);

    func(argc, argv);

    fflush(stdout);
    fflush(stderr);

    dup2(stdout_copy, fileno(stdout));
    dup2(stderr_copy, fileno(stderr));
    close(stdout_copy);
    close(stderr_copy);

    ssize_t out_len = read(out_pipe[0], out_buf, out_size - 1);
    ssize_t err_len = read(err_pipe[0], err_buf, err_size - 1);
    close(out_pipe[0]);
    close(err_pipe[0]);

    if (out_len < 0 || err_len < 0) return -1;

    out_buf[out_len] = '\0';
    err_buf[err_len] = '\0';

    return 0;
}

void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

// 1. اختبار argc != 2
void test_wrong_argc() {
    char out[1024], err[1024];
    char* argv[] = { "program" };
    int argc = 1;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_wrong_argc", 0);
        return;
    }
    if (strstr(err, "Usage:") != NULL)
        print_result("test_wrong_argc", 1);
    else
        print_result("test_wrong_argc", 0);
}

// 2. اختبار ملف موجود (تأكد من وجود الملف ومساره صحيح)
void test_file_exists() {
    const char* filepath = "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task65_Test/testFile.txt";
    char out[8192], err[1024];
    char* argv[] = { "program", (char*)filepath };
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_file_exists", 0);
        return;
    }
    // نتوقع لا خطأ في err، ومخرجات غير فارغة
    if (strlen(err) == 0 && strlen(out) > 0)
        print_result("test_file_exists", 1);
    else
        print_result("test_file_exists", 0);
}

// 3. اختبار ملف غير موجود
void test_file_not_exist() {
    char out[1024], err[1024];
    char* argv[] = { "program", "nonexistentfile.txt" };
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_file_not_exist", 0);
        return;
    }
    // نتوقع رسالة خطأ في stderr تحتوي على "Error: Unable to open file"
    if (strstr(err, "Error: Unable to open file") != NULL)
        print_result("test_file_not_exist", 1);
    else
        print_result("test_file_not_exist", 0);
}

// 4. اختبار اسم ملف غير صالح (غير مغطى في الكود الحالي)
void test_invalid_filename() {
    print_result("test_invalid_filename", 0);
}

int main() {
    test_wrong_argc();
    test_file_exists();
    test_file_not_exist();
    test_invalid_filename();
    return 0;
}
