#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // pipe, dup2, read, close
#include <errno.h>

void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return;
    }

    const char* filePath = argv[1];

    if (remove(filePath) == 0) {
        printf("File %s has been deleted.\n", filePath);
    } else {
        printf("Failed to delete the file.\n");
    }
}

// Capture stdout and stderr for testing
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

// Test cases

void test_wrong_argc() {
    char out[1024], err[1024];
    char* argv[] = { "program" };
    int argc = 1;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_wrong_argc", 0);
        return;
    }
    if (strstr(out, "Usage:") != NULL)
        print_result("test_wrong_argc", 1);
    else
        print_result("test_wrong_argc", 0);
}

void test_file_deleted_success() {
    // Create a temp file to delete
    const char* test_file = "/tmp/test_file_to_delete.tmp";
    FILE *f = fopen(test_file, "w");
    if (f) {
        fputs("test", f);
        fclose(f);
    }

    char out[1024], err[1024];
    char* argv[] = { "program", (char*)test_file };
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_file_deleted_success", 0);
        return;
    }
    if (strstr(out, "has been deleted.") != NULL)
        print_result("test_file_deleted_success", 1);
    else
        print_result("test_file_deleted_success", 0);
}

void test_file_not_found() {
    char out[1024], err[1024];
    char* argv[] = { "program", "/tmp/nonexistent_file_abcxyz.tmp" };
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_file_not_found", 0);
        return;
    }
    if (strstr(out, "Failed to delete the file.") != NULL)
        print_result("test_file_not_found", 1);
    else
        print_result("test_file_not_found", 0);
}

int main() {
    test_wrong_argc();
    test_file_deleted_success();
    test_file_not_found();
    return 0;
}
