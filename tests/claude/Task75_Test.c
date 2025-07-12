#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

int capture_output(int argc, char* argv[], char* out_buf, size_t out_size, char* err_buf, size_t err_size);

// The program_main is the main logic, to be tested
void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return;
    }

    char* endptr;
    long pid = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || pid <= 0) {
        fprintf(stderr, "Invalid PID. Please provide a valid positive integer.\n");
        return;
    }

    if (kill((pid_t)pid, SIGKILL) == 0) {
        printf("Process with PID %ld has been terminated.\n", pid);
    } else {
        fprintf(stderr, "Failed to terminate process with PID %ld: %s\n", pid, strerror(errno));
    }
}

// Capture stdout and stderr for testing
int capture_output(int argc, char* argv[], char* out_buf, size_t out_size, char* err_buf, size_t err_size) {
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

    program_main(argc, argv);

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

// Test cases strictly from python code

int test_wrong_argc() {
    char out[1024], err[1024];
    char* argv[] = {"program"};
    int argc = 1;
    if (capture_output(argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        return 0;
    }
    return (strstr(err, "Usage:") != NULL);
}

int test_invalid_pid() {
    char out[1024], err[1024];
    char* argv[] = {"program", "notanint"};
    int argc = 2;
    if (capture_output(argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        return 0;
    }
    return (strstr(err, "Invalid PID") != NULL);
}

int test_valid_pid_permission_denied_or_killed() {
    // This test requires an actual PID, which may or may not exist or be killable,
    // so just test that output contains either success message or failure message with permission or lookup errors
    char out[1024], err[1024];
    // Using PID 1 (usually init/systemd) for test - likely permission denied or fail
    char pid_str[] = "1";
    char* argv[] = {"program", pid_str};
    int argc = 2;
    if (capture_output(argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        return 0;
    }
    // Accept any of the expected outputs
    if (strstr(out, "terminated") != NULL) return 1;
    if (strstr(err, "Failed to terminate") != NULL) return 1;
    return 0;
}

int main() {
    print_result("test_wrong_argc", test_wrong_argc());
    print_result("test_invalid_pid", test_invalid_pid());
    print_result("test_valid_pid_permission_denied_or_killed", test_valid_pid_permission_denied_or_killed());
    return 0;
}
