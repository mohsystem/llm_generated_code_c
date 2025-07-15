#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>
#include <sqlite3.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PORT 8888
#define MAX_OUTPUT 8192

// --- Original handler unchanged ---
static int answer_to_connection(void *cls, struct MHD_Connection *connection,
                                const char *url, const char *method,
                                const char *version, const char *upload_data,
                                size_t *upload_data_size, void **con_cls) {
    sqlite3 *db;
    sqlite3_open("example.db", &db);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, email TEXT);", 0, 0, 0);

    const char *name = MHD_lookup_connection_value(connection, MHD_POSTDATA_KIND, "name");
    const char *email = MHD_lookup_connection_value(connection, MHD_POSTDATA_KIND, "email");

    if (name && email) {
        char *sql = sqlite3_mprintf("INSERT INTO users (name, email) VALUES (%Q, %Q);", name, email);
        sqlite3_exec(db, sql, 0, 0, 0);
        sqlite3_free(sql);

        const char *response_str = "User added successfully!";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(response_str), (void *)response_str, MHD_RESPMEM_PERSISTENT);
        sqlite3_close(db);
        return MHD_queue_response(connection, MHD_HTTP_OK, response);
    }

    sqlite3_close(db);
    return MHD_NO;
}

// --- Helper: initialize DB cleanly ---
int init_db() {
    sqlite3 *db;
    int rc = sqlite3_open("example.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    char *sql_drop = "DROP TABLE IF EXISTS users;";
    rc = sqlite3_exec(db, sql_drop, 0, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to drop table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    char *sql_create = "CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, email TEXT);";
    rc = sqlite3_exec(db, sql_create, 0, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    sqlite3_close(db);
    return 1;
}

// --- Simulate POST request by inserting data directly ---
int test_insert_user(const char* name, const char* email, char* output, size_t max_output) {
    if (!init_db()) {
        snprintf(output, max_output, "DB init failed");
        return 1;
    }

    if (!name || !email) {
        snprintf(output, max_output, "Missing name or email");
        return 1;
    }

    sqlite3 *db;
    int rc = sqlite3_open("example.db", &db);
    if (rc != SQLITE_OK) {
        snprintf(output, max_output, "Cannot open DB");
        sqlite3_close(db);
        return 1;
    }

    char *sqlInsert = sqlite3_mprintf("INSERT INTO users (name, email) VALUES (%Q, %Q);", name, email);
    rc = sqlite3_exec(db, sqlInsert, 0, 0, NULL);
    sqlite3_free(sqlInsert);
    sqlite3_close(db);

    if (rc != SQLITE_OK) {
        snprintf(output, max_output, "Insert failed");
        return 1;
    }

    snprintf(output, max_output, "User added successfully!");
    return 0;
}

void run_tests() {
    struct {
        const char* name;
        const char* email;
        const char* expected_response;
        int expect_success;
        const char* description;
    } tests[] = {
        {"John Doe", "john.doe@example.com", "User added successfully!", 1, "Test 1: Insert valid user"},
        {"Jane Doe", "jane.doe@example.com", "User added successfully!", 1, "Test 2: Insert another valid user"},
        {"John Doe", "john.doe@example.com", "Insert failed", 0, "Test 3: Insert duplicate email user (should fail but no UNIQUE constraint in table)"},
        {"", "no.name@example.com", "User added successfully!", 1, "Test 4: Insert user with empty name (no validation)"},
        {"No Email", "", "User added successfully!", 1, "Test 5: Insert user with empty email (no validation)"},
        {NULL, NULL, "Missing name or email", 0, "Test 6: Insert NULL values (should fail)"},
    };

    char output[MAX_OUTPUT];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i=0; i<n; i++) {
        int ret = test_insert_user(tests[i].name, tests[i].email, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if ((ret == 0 && strstr(output, tests[i].expected_response)) ||
            (ret != 0 && strstr(output, tests[i].expected_response))) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected: %s\nGot: %s\n", tests[i].expected_response, output);
        }
    }
}

int main() {
    run_tests();
    return 0;
}
