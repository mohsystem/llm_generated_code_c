#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

// إعادة تعريف createTable من كودك
void createTable() {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open("users.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS users("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "NAME TEXT NOT NULL, "
                "EMAIL TEXT NOT NULL, "
                "PASSWORD TEXT NOT NULL);";

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    sqlite3_close(db);
}

// إعادة تعريف registerUser من كودك لكن بدون طباعة، ونعيد القيمة 1 نجاح و0 فشل مع رسالة
int registerUser(const char *name, const char *email, const char *password, char *out_msg, size_t msg_size) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open("users.db", &db);
    if (rc) {
        snprintf(out_msg, msg_size, "Can't open database");
        return 0;
    }

    char sql[200];
    snprintf(sql, sizeof(sql), "INSERT INTO users (NAME, EMAIL, PASSWORD) VALUES ('%s', '%s', '%s');", name, email, password);

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        if (errMsg && strstr(errMsg, "UNIQUE constraint failed") != NULL) {
            snprintf(out_msg, msg_size, "Email already exists. Registration failed.");
        } else {
            snprintf(out_msg, msg_size, "SQL error: %s", errMsg ? errMsg : "Unknown error");
        }
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 0;
    }

    snprintf(out_msg, msg_size, "Registration successful!");
    sqlite3_close(db);
    return 1;
}

// اختبار وجود الجدول
int test_create_table() {
    createTable();

    sqlite3 *db;
    int rc = sqlite3_open("users.db", &db);
    if (rc) return 0;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_step(stmt);
    int exists = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return exists;
}

// اختبار تسجيل مستخدم جديد
int test_register_new_user() {
    char msg[100];
    // مسح الجدول للبدء من جديد
    sqlite3 *db;
    if (sqlite3_open("users.db", &db) == SQLITE_OK) {
        sqlite3_exec(db, "DELETE FROM users;", 0, 0, 0);
        sqlite3_close(db);
    }

    int res = registerUser("John Doe", "johndoe@example.com", "password123", msg, sizeof(msg));
    return (res == 1 && strcmp(msg, "Registration successful!") == 0);
}

// اختبار إدخال بريد مكرر
int test_register_duplicate_email() {
    char msg[100];
    // مسح الجدول للبدء من جديد
    sqlite3 *db;
    if (sqlite3_open("users.db", &db) == SQLITE_OK) {
        sqlite3_exec(db, "DELETE FROM users;", 0, 0, 0);
        sqlite3_close(db);
    }

    registerUser("John Doe", "johndoe@example.com", "password123", msg, sizeof(msg));
    int res = registerUser("Jane Doe", "johndoe@example.com", "password456", msg, sizeof(msg));
    return (res == 0 && strcmp(msg, "Email already exists. Registration failed.") == 0);
}

// اختبار التحقق من بيانات المستخدم بعد التسجيل
int test_register_user_validity() {
    char msg[100];
    // مسح الجدول للبدء من جديد
    sqlite3 *db;
    if (sqlite3_open("users.db", &db) != SQLITE_OK) return 0;
    sqlite3_exec(db, "DELETE FROM users;", 0, 0, 0);

    int res = registerUser("Alice Smith", "alice@example.com", "password789", msg, sizeof(msg));
    if (!res) {
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "SELECT NAME, EMAIL, PASSWORD FROM users WHERE EMAIL = 'alice@example.com';";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
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
    sqlite3_close(db);
    return pass;
}

int main() {
    printf("Test create_table: %s\n", test_create_table() ? "PASS" : "FAIL");
    printf("Test register_new_user: %s\n", test_register_new_user() ? "PASS" : "FAIL");
    printf("Test register_duplicate_email: %s\n", test_register_duplicate_email() ? "PASS" : "FAIL");
    printf("Test register_user_validity: %s\n", test_register_user_validity() ? "PASS" : "FAIL");
    return 0;
}
