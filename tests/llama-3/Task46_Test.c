#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

// مساعدة لحذف نهاية السطر من سترينغ C
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n')
        str[len-1] = '\0';
}

// إعادة إنشاء الجدول حسب كودك
int createTable(sqlite3 *db) {
    char *errMsg = 0;
    int rc = sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS users (name TEXT, email TEXT PRIMARY KEY, password TEXT);",
        0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

// تسجيل مستخدم مع حذف نهاية السطر من الإدخالات
int registerUser(sqlite3 *db, const char *name_in, const char *email_in, const char *password_in, char *out_msg, size_t msg_size) {
    char name[101], email[101], password[101];
    strncpy(name, name_in, 100);
    strncpy(email, email_in, 100);
    strncpy(password, password_in, 100);
    name[100] = email[100] = password[100] = '\0';
    trim_newline(name);
    trim_newline(email);
    trim_newline(password);

    char sql[512];
    snprintf(sql, sizeof(sql), "INSERT INTO users VALUES ('%s', '%s', '%s')", name, email, password);

    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        if (errMsg && strstr(errMsg, "UNIQUE constraint failed") != NULL) {
            snprintf(out_msg, msg_size, "Email already exists. Registration failed.");
        } else {
            snprintf(out_msg, msg_size, "SQL error: %s", errMsg ? errMsg : "Unknown error");
        }
        sqlite3_free(errMsg);
        return 0;
    }

    snprintf(out_msg, msg_size, "Registration successful!");
    return 1;
}

// اختبار وجود جدول
int test_create_table(sqlite3 *db) {
    if (createTable(db) != SQLITE_OK) return 0;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;

    int rc = sqlite3_step(stmt);
    int exists = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

// اختبار تسجيل مستخدم جديد
int test_register_new_user(sqlite3 *db) {
    char msg[100];
    sqlite3_exec(db, "DELETE FROM users;", 0, 0, 0);
    int res = registerUser(db, "John Doe", "john@example.com", "password123\n", msg, sizeof(msg));
    return (res == 1 && strcmp(msg, "Registration successful!") == 0);
}

// اختبار تسجيل بريد مكرر
int test_register_duplicate_email(sqlite3 *db) {
    char msg[100];
    sqlite3_exec(db, "DELETE FROM users;", 0, 0, 0);
    registerUser(db, "John Doe", "john@example.com", "password123\n", msg, sizeof(msg));
    int res = registerUser(db, "Jane Doe", "john@example.com", "password456\n", msg, sizeof(msg));
    return (res == 0 && strcmp(msg, "Email already exists. Registration failed.") == 0);
}

// اختبار صحة بيانات مستخدم
int test_register_user_validity(sqlite3 *db) {
    char msg[100];
    sqlite3_exec(db, "DELETE FROM users;", 0, 0, 0);
    int res = registerUser(db, "Alice Smith", "alice@example.com", "password789\n", msg, sizeof(msg));
    if (!res) return 0;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT name, email, password FROM users WHERE email='alice@example.com';";
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
    int rc = sqlite3_open("users.db", &db);
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
