#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <openssl/sha.h>

char* hashPassword(const char* password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);
    char* hashString = malloc((SHA256_DIGEST_LENGTH * 2) + 1);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashString + (i * 2), "%02x", hash[i]);
    }
    hashString[SHA256_DIGEST_LENGTH * 2] = '\0';
    return hashString;
}

int updatePassword(const char* username, const char* new_password) {
    sqlite3 *db;
    int rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open DB: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // إنشاء جدول users لو مش موجود (مطابق بايثون)
    const char* create_sql = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);";
    char* err_msg = NULL;
    rc = sqlite3_exec(db, create_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error creating table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    // جهز statement لتحديث الباسورد
    sqlite3_stmt *stmt;
    const char* update_sql = "UPDATE users SET password = ? WHERE username = ?;";
    rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    char* hashed = hashPassword(new_password);
    sqlite3_bind_text(stmt, 1, hashed, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // تحقق كم صف تأثر (مطابق بايثون)
    int changes = sqlite3_changes(db);
    if (changes == 0) {
        // لم يحدث تحديث، أدخل مستخدم جديد مع الباسورد
        const char* insert_sql = "INSERT INTO users (username, password) VALUES (?, ?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            free(hashed);
            sqlite3_close(db);
            return 0;
        }
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, hashed, -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            free(hashed);
            sqlite3_close(db);
            return 0;
        }
    }

    free(hashed);
    sqlite3_close(db);
    return 1;
}

int verifyPassword(const char* username, const char* password) {
    sqlite3 *db;
    int rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    const char* sql = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);

    int result = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* db_hash = sqlite3_column_text(stmt, 0);
        char* expected_hash = hashPassword(password);
        if (strcmp((const char*)db_hash, expected_hash) == 0) {
            result = 1;
        }
        free(expected_hash);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}

void run_test_case(const char* username, const char* newPassword) {
    int updated = updatePassword(username, newPassword);
    int verified = verifyPassword(username, newPassword);
    if (updated && verified) {
        printf("Test case (username='%s', newPassword='%s'): PASS\n", username, newPassword);
    } else {
        printf("Test case (username='%s', newPassword='%s'): FAIL\n", username, newPassword);
    }
}

int main() {
    run_test_case("user1", "password123");
    run_test_case("user2", "mypassword");
    run_test_case("user1", "newpassword");
    run_test_case("user3", "123456");
    run_test_case("", "");

    return 0;
}
