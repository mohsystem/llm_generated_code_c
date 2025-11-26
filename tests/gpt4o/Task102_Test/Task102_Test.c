#include <stdio.h>
#include <openssl/sha.h>
#include <sqlite3.h>
#include <string.h>

void hashPassword(const char* password, char* outputBuffer) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

int updatePassword(const char* username, const char* password) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    char hashedPassword[65];
    hashPassword(password, hashedPassword);

    // إنشاء الجدول إذا لم يكن موجوداً (حسب كودك)
    char sql[256];
    sprintf(sql, "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);");
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 0;
    }

    // تحديث كلمة السر
    sprintf(sql, "UPDATE users SET password = '%s' WHERE username = '%s';", hashedPassword, username);
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 0;
    }

    // إذا لم يتم تحديث أي صفوف، نضيف المستخدم مع كلمة المرور (مطابق البايثون)
    int changes = sqlite3_changes(db);
    if (changes == 0) {
        sprintf(sql, "INSERT INTO users (username, password) VALUES ('%s', '%s');", username, hashedPassword);
        rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return 0;
        }
    }

    sqlite3_close(db);
    return 1;
}

int verifyPassword(const char* username, const char* password) {
    sqlite3* db;
    if (sqlite3_open("users.db", &db) != SQLITE_OK) {
        return 0;
    }

    const char* sql = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    int result = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* db_hash = sqlite3_column_text(stmt, 0);
        char expected_hash[65];
        hashPassword(password, expected_hash);
        if (strcmp((const char*)db_hash, expected_hash) == 0) {
            result = 1;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}

void run_test_case(const char* username, const char* newPassword) {
    int update_result = updatePassword(username, newPassword);
    int verify_result = verifyPassword(username, newPassword);

    if (update_result && verify_result) {
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
