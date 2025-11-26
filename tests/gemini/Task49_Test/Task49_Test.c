#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <microhttpd.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX_OUTPUT 8192

// --- Your original handler unchanged ---
static int handle_request(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls) {
    if (strcmp(method, "POST") == 0 && strcmp(url, "/users") == 0) {
        const char *name = NULL, *email = NULL;
        char *data = (char *) upload_data;

        while (*data) {
            char *key = data;
            char *value = strchr(key, '=');
            if (value) {
                *value++ = '\0';
                if (strcmp(key, "name") == 0) {
                    name = value;
                } else if (strcmp(key, "email") == 0) {
                    email = value;
                }
                data = strchr(value, '&');
                if (data) {
                    *data++ = '\0';
                } else {
                    break;
                }
            }
        }

        if (name && email) {
            sqlite3 *db;
            char *zErrMsg = 0;
            int rc = sqlite3_open("users.db", &db);
            if (rc) {
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return MHD_NO;
            }

            char *sqlCreateTable = "CREATE TABLE IF NOT EXISTS users ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "name TEXT NOT NULL,"
                                   "email TEXT NOT NULL UNIQUE"
                                   ");";
            rc = sqlite3_exec(db, sqlCreateTable, 0, 0, &zErrMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
                sqlite3_close(db);
                return MHD_NO;
            }

            char sqlInsert[256];
            sprintf(sqlInsert, "INSERT INTO users (name, email) VALUES ('%s', '%s')", name, email);
            rc = sqlite3_exec(db, sqlInsert, 0, 0, &zErrMsg);
            if (rc != SQLITE_OK) {
                if (strstr(zErrMsg, "UNIQUE constraint failed") != NULL) {
                    sqlite3_free(zErrMsg);
                    sqlite3_close(db);
                    struct MHD_Response *response =
                            MHD_create_response_from_buffer(strlen("Email already exists"),
                                                           "Email already exists",
                                                           MHD_RESPMEM_MUST_COPY);
                    MHD_add_response_header(response, "Content-Type", "text/plain");
                    int ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                    MHD_destroy_response(response);
                    return ret;
                } else {
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                    sqlite3_close(db);
                    return MHD_NO;
                }
            }

            sqlite3_close(db);

            struct MHD_Response *response =
                    MHD_create_response_from_buffer(strlen("User created successfully"),
                                                   "User created successfully",
                                                   MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "text/plain");
            int ret = MHD_queue_response(connection, MHD_HTTP_CREATED, response);
            MHD_destroy_response(response);
            return ret;
        } else {
            struct MHD_Response *response =
                    MHD_create_response_from_buffer(strlen("Name and email are required"),
                                                   "Name and email are required",
                                                   MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "text/plain");
            int ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
            MHD_destroy_response(response);
            return ret;
        }
    }
    return MHD_NO;
}

// --- Helper: initialize DB with clean state ---
int init_db() {
    sqlite3 *db;
    int rc = sqlite3_open("users.db", &db);
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
    char *sql_create = "CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, email TEXT NOT NULL UNIQUE);";
    rc = sqlite3_exec(db, sql_create, 0, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    sqlite3_close(db);
    return 1;
}

// --- Simulated test POST request ---
int test_insert_user(const char* name, const char* email, char* output, size_t max_output) {
    if (!init_db()) {
        snprintf(output, max_output, "DB init failed");
        return 1;
    }

    if (!name || !email) {
        snprintf(output, max_output, "Name and email are required");
        return 1;
    }

    sqlite3 *db;
    int rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        snprintf(output, max_output, "Cannot open DB");
        sqlite3_close(db);
        return 1;
    }

    char sqlInsert[256];
    snprintf(sqlInsert, sizeof(sqlInsert), "INSERT INTO users (name, email) VALUES ('%s', '%s')", name, email);
    char *zErrMsg = NULL;
    rc = sqlite3_exec(db, sqlInsert, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        if (strstr(zErrMsg, "UNIQUE constraint failed") != NULL) {
            snprintf(output, max_output, "Email already exists");
            sqlite3_free(zErrMsg);
            sqlite3_close(db);
            return 1;
        } else {
            snprintf(output, max_output, "SQL error: %s", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_close(db);
            return 1;
        }
    }
    sqlite3_close(db);
    snprintf(output, max_output, "User created successfully");
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
        {"John Doe", "john.doe@example.com", "User created successfully", 1, "Test 1: Insert valid user"},
        {"Jane Doe", "jane.doe@example.com", "User created successfully", 1, "Test 2: Insert another valid user"},
        {"John Doe", "john.doe@example.com", "Email already exists", 0, "Test 3: Insert duplicate email user (should fail)"},
        {"", "no.name@example.com", "User created successfully", 1, "Test 4: Insert user with empty name (no validation)"},
        {"No Email", "", "User created successfully", 1, "Test 5: Insert user with empty email (no validation)"},
        {NULL, NULL, "Name and email are required", 0, "Test 6: Insert NULL values (should fail)"},
    };

    char output[MAX_OUTPUT];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < n; i++) {
        int ret = test_insert_user(tests[i].name, tests[i].email, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if (ret == 0 && strstr(output, tests[i].expected_response) != NULL) {
            printf("PASS\n");
        } else if (ret != 0 && strstr(output, tests[i].expected_response) != NULL) {
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
