#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h>   // pipe, dup2, read, close

typedef struct {
    int id;
    char username[50];
    char name[100];
    char email[100];
    char phone[20];
} CustomerInfo;

CustomerInfo* getCustomerInfo(const char* username) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    CustomerInfo* customer = NULL;

    if (sqlite3_open("/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task67_Test/customer_database.db", &db) == SQLITE_OK) {
        const char* sql = "SELECT * FROM customer WHERE username = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                customer = (CustomerInfo*)malloc(sizeof(CustomerInfo));
                customer->id = sqlite3_column_int(stmt, 0);
                strncpy(customer->username, (const char*)sqlite3_column_text(stmt, 1), sizeof(customer->username) - 1);
                customer->username[sizeof(customer->username) - 1] = '\0';
                strncpy(customer->name, (const char*)sqlite3_column_text(stmt, 2), sizeof(customer->name) - 1);
                customer->name[sizeof(customer->name) - 1] = '\0';
                strncpy(customer->email, (const char*)sqlite3_column_text(stmt, 3), sizeof(customer->email) - 1);
                customer->email[sizeof(customer->email) - 1] = '\0';
                strncpy(customer->phone, (const char*)sqlite3_column_text(stmt, 4), sizeof(customer->phone) - 1);
                customer->phone[sizeof(customer->phone) - 1] = '\0';
            }

            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }

    return customer;
}

void program_main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <customerusername>\n", argv[0]);
        return;
    }

    const char* username = argv[1];
    CustomerInfo* customerInfo = getCustomerInfo(username);

    if (customerInfo) {
        printf("Customer Information:\n");
        printf("ID: %d\n", customerInfo->id);
        printf("Username: %s\n", customerInfo->username);
        printf("Name: %s\n", customerInfo->name);
        printf("Email: %s\n", customerInfo->email);
        printf("Phone: %s\n", customerInfo->phone);
        free(customerInfo);
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
    if (strstr(err, "Usage:") != NULL || strstr(out, "Usage:") != NULL)
        print_result("test_wrong_argc", 1);
    else
        print_result("test_wrong_argc", 0);
}

void test_customer_found() {
    char out[4096], err[1024];
    char* argv[] = { "program", "existing_username" };
    int argc = 2;
    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_customer_found", 0);
        return;
    }
    printf("DEBUG OUTPUT (test_customer_found):\n%s\n", out); // Debug output to inspect result
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
