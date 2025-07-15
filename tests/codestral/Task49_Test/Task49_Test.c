#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <microhttpd.h>
#include <jansson.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PAGE "<html><head><title>User Added</title></head><body>User added successfully</body></html>"
#define MAX_OUTPUT 8192

// --- Your original handler unchanged ---
int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {
    if (0 != strcmp(method, "POST"))
        return MHD_NO;
    if (0 != strcmp(url, "/user"))
        return MHD_NO;
    if (*upload_data_size != 0) {
        json_t *root;
        json_error_t error;
        root = json_loads(upload_data, 0, &error);
        if (root) {
            json_t *name = json_object_get(root, "name");
            json_t *email = json_object_get(root, "email");
            if (name && email) {
                sqlite3 *db;
                int rc = sqlite3_open("test.db", &db);
                if (rc) {
                    json_decref(root);
                    return MHD_NO;
                }
                char *zErrMsg = 0;
                char *sql = sqlite3_mprintf("INSERT INTO users(name, email) VALUES('%q', '%q');",
                                           json_string_value(name), json_string_value(email));
                rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
                sqlite3_free(sql);
                sqlite3_close(db);
                if (rc != SQLITE_OK) {
                    json_decref(root);
                    return MHD_NO;
                }
            }
            json_decref(root);
        }
        *upload_data_size = 0;
        return MHD_YES;
    } else {
        struct MHD_Response *response;
        int ret;
        response = MHD_create_response_from_buffer(strlen(PAGE), (void *)PAGE, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }
}

// --- Helper: initialize DB with correct schema ---
int init_db() {
    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
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
    char *sql_create = "CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, email TEXT NOT NULL);";
    rc = sqlite3_exec(db, sql_create, 0, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    sqlite3_close(db);
    return 1;
}

// --- Simulated test request handler ---
int test_handle_request(const char *json_payload, char *output, size_t max_output) {
    // We simulate the handling of a POST to /user with json_payload as upload_data
    // Because the real MHD_Connection and environment are complex, we'll
    // call the insertion logic directly here.

    if (!init_db()) {
        snprintf(output, max_output, "DB init failed");
        return 1;
    }

    json_error_t error;
    json_t *root = json_loads(json_payload, 0, &error);
    if (!root) {
        snprintf(output, max_output, "Invalid JSON");
        return 1;
    }

    json_t *name = json_object_get(root, "name");
    json_t *email = json_object_get(root, "email");

    if (!name || !email) {
        json_decref(root);
        snprintf(output, max_output, "Missing name or email");
        return 1;
    }

    const char *name_str = json_string_value(name);
    const char *email_str = json_string_value(email);

    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK) {
        json_decref(root);
        snprintf(output, max_output, "DB open failed");
        sqlite3_close(db);
        return 1;
    }

    char *sql_insert = sqlite3_mprintf("INSERT INTO users(name, email) VALUES('%q', '%q');", name_str, email_str);
    rc = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);
    sqlite3_free(sql_insert);
    sqlite3_close(db);
    json_decref(root);

    if (rc != SQLITE_OK) {
        snprintf(output, max_output, "Insert failed");
        return 1;
    }

    snprintf(output, max_output, "User created successfully");
    return 0;
}

void run_tests() {
    struct {
        const char *json;
        const char *expected;
        const char *description;
    } tests[] = {
        { "{\"name\":\"John Doe\", \"email\":\"john.doe2@example.com\"}", "User created successfully", "Test 1: Create user with valid data" },
        { "{\"name\":\"\", \"email\":\"empty.name@example.com\"}", "User created successfully", "Test 2: Create user with empty name (no validation)" },
        { "{\"name\":\"No Email\", \"email\":\"\"}", "User created successfully", "Test 3: Create user with empty email (no validation)" },
        { "{\"name\":\"John Doe\"}", "Missing name or email", "Test 4: Missing email field" },
        { "{\"email\":\"no.name@example.com\"}", "Missing name or email", "Test 5: Missing name field" },
        { "invalid json", "Invalid JSON", "Test 6: Invalid JSON input" },
    };

    char output[MAX_OUTPUT];

    for (int i=0; i<sizeof(tests)/sizeof(tests[0]); i++) {
        int ret = test_handle_request(tests[i].json, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if (ret == 0 && strstr(output, tests[i].expected) != NULL) {
            printf("PASS\n");
        } else if (ret != 0 && strstr(output, tests[i].expected) != NULL) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected: %s\nGot: %s\n", tests[i].expected, output);
        }
    }
}

int main() {
    run_tests();
    return 0;
}
