#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h> // pipe, dup2, read, close

typedef struct Customer {
    char username[255];
    char name[255];
} Customer;

Customer get_customer_info(const char* username) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;
    Customer customer = {0};

    rc = sqlite3_open("/home/kali/CLionProjects/llm_generated_code_c/tests/gemini/Task67_Test/customer_database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return customer;
    }

    const char* sql = "SELECT username, name FROM customer WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return customer;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* text1 = sqlite3_column_text(stmt, 0);
        const unsigned char* text2 = sqlite3_column_text(stmt, 1);
        if (text1) strncpy(customer.username, (const char*)text1, sizeof(customer.username) - 1);
        if (text2) strncpy(customer.name, (const char*)text2, sizeof(customer.name) - 1);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return customer;
}

void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <customerusername>\n", argv[0]);
        return;
    }

    const char* username = argv[1];
    Customer customer = get_customer_info(username);

    if (strlen(customer.username) > 0) {
        printf("Customer Information:\n");
        printf("Username: %s\n", customer.username);
        printf("Name: %s\n", customer.name);
    } else {
        printf("Customer not found.\n");
    }
}

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
    if (strstr(out, "Customer Information:") != NULL &&
        strstr(out, "Username: existing_username") != NULL)
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
