#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h> // pipe, dup2, read, close

void get_customer_info(const char *username) {
    sqlite3 *db;
    int rc = sqlite3_open("/home/kali/CLionProjects/llm_generated_code_c/tests/gemini/Task67_Test/customer_database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    const char *query = "SELECT * FROM customer WHERE username = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = 1;
        for (int col = 0; col < sqlite3_column_count(stmt); col++) {
            const unsigned char *text = sqlite3_column_text(stmt, col);
            printf("%s", text ? (const char*)text : "NULL");
            if (col < sqlite3_column_count(stmt) - 1) printf(" | ");
        }
        printf("\n");
    }
    if (!found) {
        printf("Customer not found.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <customerusername>\n", argv[0]);
        return;
    }
    get_customer_info(argv[1]);
}

// Capture stdout and stderr for tests
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
    char* argv[] = {"program"};
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
    char* argv[] = {"program", "existing_username"};
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_customer_found", 0);
        return;
    }
    if (strstr(out, "existing_username") != NULL && strchr(out, '|') != NULL)
        print_result("test_customer_found", 1);
    else
        print_result("test_customer_found", 0);
}

void test_customer_not_found() {
    char out[1024], err[1024];
    char* argv[] = {"program", "nonexistent_user"};
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_customer_not_found", 0);
        return;
    }
    if (strstr(out, "Customer not found.") != NULL)
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
