#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_LENGTH 100

// تابع إنشاء الجدول كما هو في الكود الأصلي
void createTable(sqlite3 *db) {
    char *errMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS users "
                      "(name TEXT, email TEXT PRIMARY KEY, password TEXT);";

    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

// دالة تسجيل مستخدم مع معطيات مدخلة (لا تدخل من المستخدم)
const char* registerUserWithData(sqlite3 *db, const char *name, const char *email, const char *password) {
    char *sql = "INSERT INTO users (name, email, password) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        return "SQL error: prepare failed";
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        if (strstr(sqlite3_errmsg(db), "UNIQUE") || strstr(sqlite3_errmsg(db), "constraint")) {
            return "Email already exists. Registration failed.";
        } else {
            return "Execution failed.";
        }
    }
    return "Registration successful!";
}

// تحقق من وجود جدول users
int test_create_table(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) return 0;

    rc = sqlite3_step(stmt);
    int exists = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

// تحقق من تسجيل مستخدم جديد
int test_register_new_user(sqlite3 *db) {
    const char* res = registerUserWithData(db, "John Doe", "john@example.com", "password123");
    return strcmp(res, "Registration successful!") == 0;
}

// تحقق من تسجيل مستخدم ببريد مكرر
int test_register_duplicate_email(sqlite3 *db) {
    registerUserWithData(db, "John Doe", "john@example.com", "password123");
    const char* res = registerUserWithData(db, "Jane Doe", "john@example.com", "password456");
    return strcmp(res, "Email already exists. Registration failed.") == 0;
}

// تحقق من بيانات مستخدم مسجل
int test_register_user_validity(sqlite3 *db) {
    registerUserWithData(db, "Alice Smith", "alice@example.com", "password789");

    sqlite3_stmt *stmt;
    const char *sql = "SELECT name, email, password FROM users WHERE email = ?;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) return 0;

    sqlite3_bind_text(stmt, 1, "alice@example.com", -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 0;
    }

    const unsigned char *name = sqlite3_column_text(stmt, 0);
    const unsigned char *email = sqlite3_column_text(stmt, 1);
    const unsigned char *password = sqlite3_column_text(stmt, 2);

    int pass = 1;
    pass &= (strcmp((const char*)name, "Alice Smith") == 0);
    pass &= (strcmp((const char*)email, "alice@example.com") == 0);
    pass &= (strcmp((const char*)password, "password789") == 0);

    sqlite3_finalize(stmt);
    return pass;
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    createTable(db);

    printf("Test create_table: %s\n", test_create_table(db) ? "PASS" : "FAIL");
    printf("Test register_new_user: %s\n", test_register_new_user(db) ? "PASS" : "FAIL");
    printf("Test register_duplicate_email: %s\n", test_register_duplicate_email(db) ? "PASS" : "FAIL");
    printf("Test register_user_validity: %s\n", test_register_user_validity(db) ? "PASS" : "FAIL");

    sqlite3_close(db);
    return 0;
}
