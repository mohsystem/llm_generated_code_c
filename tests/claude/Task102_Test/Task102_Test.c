#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <sqlite3.h>

void hash_password(const char* password, char* hashed_password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hashed_password[i * 2], "%02x", hash[i]);
    }
    hashed_password[SHA256_DIGEST_LENGTH * 2] = '\0';
}

void update_password(const char* username, const char* new_password) {
    char hashed_password[SHA256_DIGEST_LENGTH * 2 + 1];
    hash_password(new_password, hashed_password);

    sqlite3* db;
    char* err_msg = 0;

    if (sqlite3_open("users.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    char* create_table_sql = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);";
    if (sqlite3_exec(db, create_table_sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    char update_sql[512];
    snprintf(update_sql, sizeof(update_sql), "UPDATE users SET password = '%s' WHERE username = '%s';", hashed_password, username);
    if (sqlite3_exec(db, update_sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    if (sqlite3_changes(db) == 0) {
        char insert_sql[512];
        snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO users (username, password) VALUES ('%s', '%s');", username, hashed_password);
        if (sqlite3_exec(db, insert_sql, 0, 0, &err_msg) != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
    }

    sqlite3_close(db);
}

int verify_password(const char* username, const char* expected_password) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int result = 0;

    if (sqlite3_open("users.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char* query = "SELECT password FROM users WHERE username = ?;";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement\n");
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* db_password = sqlite3_column_text(stmt, 0);
        if (strcmp((const char*)db_password, expected_password) == 0) {
            result = 1;  // passwords match
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}

void run_test_case(const char* username, const char* new_password) {
    char hashed_password[SHA256_DIGEST_LENGTH * 2 + 1];
    hash_password(new_password, hashed_password);

    update_password(username, new_password);

    if (verify_password(username, hashed_password)) {
        printf("Test case (username='%s', new_password='%s'): PASS\n", username, new_password);
    } else {
        printf("Test case (username='%s', new_password='%s'): FAIL\n", username, new_password);
    }
}

int main() {
    // Test cases - نفس التي قد تدخلها يدوياً في بايثون
    run_test_case("user1", "password123");
    run_test_case("user2", "mypassword");
    run_test_case("user1", "newpassword");  // تحديث لكلمة موجودة
    run_test_case("user3", "123456");
    run_test_case("user4", "");              // كلمة مرور فارغة (اختبار هام)

    return 0;
}
