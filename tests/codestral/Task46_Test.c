#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

int execute_sql(sqlite3 *db, const char *sql) {
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

int create_table(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS users "
                      "(name TEXT, email TEXT PRIMARY KEY, password TEXT);";
    return execute_sql(db, sql);
}

int insert_user(sqlite3 *db, const char *name, const char *email, const char *password, char *out_msg, size_t msg_size) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO users (name, email, password) VALUES('%s', '%s', '%s');",
             name, email, password);

    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        if (errMsg && strstr(errMsg, "UNIQUE") != NULL) {
            snprintf(out_msg, msg_size, "Email already exists. Registration failed.");
        } else {
            snprintf(out_msg, msg_size, "SQL error: %s", errMsg);
        }
        sqlite3_free(errMsg);
        return 0;
    }
    snprintf(out_msg, msg_size, "Registration successful!");
    return 1;
}

int test_create_table(sqlite3 *db) {
    if (create_table(db) != SQLITE_OK) return 0;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;
    int rc = sqlite3_step(stmt);
    int exists = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

int test_register_new_user(sqlite3 *db) {
    char msg[100];
    return insert_user(db, "John Doe", "johndoe@example.com", "password123", msg, sizeof(msg)) &&
           strcmp(msg, "Registration successful!") == 0;
}

int test_register_duplicate_email(sqlite3 *db) {
    char msg[100];
    // Insert first user
    insert_user(db, "John Doe", "johndoe@example.com", "password123", msg, sizeof(msg));
    // Insert duplicate email
    int success = insert_user(db, "Jane Doe", "johndoe@example.com", "password456", msg, sizeof(msg));
    return (!success) && (strcmp(msg, "Email already exists. Registration failed.") == 0);
}

int test_register_user_validity(sqlite3 *db) {
    char msg[100];
    insert_user(db, "Alice Smith", "alice@example.com", "password789", msg, sizeof(msg));

    sqlite3_stmt *stmt;
    const char *sql = "SELECT name, email, password FROM users WHERE email = 'alice@example.com';";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 0;
    }

    const unsigned char *name = sqlite3_column_text(stmt, 0);
    const unsigned char *email = sqlite3_column_text(stmt, 1);
    const unsigned char *password = sqlite3_column_text(stmt, 2);

    int pass = 1;
    pass &= (strcmp((const char *)name, "Alice Smith") == 0);
    pass &= (strcmp((const char *)email, "alice@example.com") == 0);
    pass &= (strcmp((const char *)password, "password789") == 0);

    sqlite3_finalize(stmt);
    return pass;
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("user_database.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    printf("Test create_table: %s\n", test_create_table(db) ? "PASS" : "FAIL");
    printf("Test register_new_user: %s\n", test_register_new_user(db) ? "PASS" : "FAIL");
    printf("Test register_duplicate_email: %s\n", test_register_duplicate_email(db) ? "PASS" : "FAIL");
    printf("Test register_user_validity: %s\n", test_register_user_validity(db) ? "PASS" : "FAIL");

    sqlite3_close(db);
    return 0;
}
