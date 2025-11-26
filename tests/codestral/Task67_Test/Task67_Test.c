#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h> // pipe, dup2, read, close

static int rows_found = 0;

static int callback(void *data, int argc, char **argv, char **azColName) {
    rows_found++;
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <customerusername>\n", argv[0]);
        return;
    }

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    rows_found = 0;

    rc = sqlite3_open("/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task67_Test/customer_database.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    sql = sqlite3_mprintf("SELECT * FROM customer WHERE username = '%q'", argv[1]);

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else if (rows_found == 0) {
        fprintf(stdout, "No customer found with username: %s\n", argv[1]);
    }

    sqlite3_free(sql);
    sqlite3_close(db);
}

// Redirect stdout and stderr and capture outputs
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

void test_customer_found() {
    char out[8192], err[1024];
    char* argv[] = { "program", "existing_username" };
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_customer_found", 0);
        return;
    }
    // Debug print to see output during test run - remove if not needed
    // printf("DEBUG OUTPUT:\n%s\n", out);
    if (strstr(out, "username") != NULL || strstr(out, "id") != NULL)
        print_result("test_customer_found", 1);
    else
        print_result("test_customer_found", 0);
}

void test_customer_not_found() {
    char out[1024], err[1024];
    char* argv[] = { "program", "nonexistent_user" };
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_customer_not_found", 0);
        return;
    }
    if (strstr(out, "No customer found") != NULL)
        print_result("test_customer_not_found", 1);
    else
        print_result("test_customer_not_found", 0);
}

int main() {
    test_wrong_argc();
    test_customer_found();
    test_customer_not_found();
    return 0;
}
