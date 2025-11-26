#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // pipe, dup2, read, close

void program_main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Invalid input. Please enter an integer.\n");
        return;
    }

    char* end;
    long num = strtol(argv[1], &end, 10);

    if (*end != '\0') {
        printf("Invalid input. Please enter an integer.\n");
    } else {
        printf("%ld\n", num);
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

void test_no_argument() {
    char out[1024], err[1024];
    char* argv[] = {"program"};
    int argc = 1;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_no_argument", 0);
        return;
    }
    if (strstr(out, "Invalid input") != NULL)
        print_result("test_no_argument", 1);
    else
        print_result("test_no_argument", 0);
}

void test_valid_integer() {
    char out[1024], err[1024];
    char* argv[] = {"program", "12345"};
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_valid_integer", 0);
        return;
    }
    if (strstr(out, "12345") != NULL)
        print_result("test_valid_integer", 1);
    else
        print_result("test_valid_integer", 0);
}

void test_invalid_integer() {
    char out[1024], err[1024];
    char* argv[] = {"program", "12abc"};
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_invalid_integer", 0);
        return;
    }
    if (strstr(out, "Invalid input") != NULL)
        print_result("test_invalid_integer", 1);
    else
        print_result("test_invalid_integer", 0);
}

int main() {
    test_no_argument();
    test_valid_integer();
    test_invalid_integer();
    return 0;
}
