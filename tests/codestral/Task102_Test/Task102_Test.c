#include <sqlite3.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

// Hash the password with SHA256 and return hex string
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

// Update password for existing user only, no insert
int updatePassword(const char* username, const char* newPassword) {
    sqlite3* DB;
    int rc = sqlite3_open("users.db", &DB);  // اسم القاعدة مضبوط هنا
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(DB));
        return 0;
    }

    char hashedPassword[SHA256_DIGEST_LENGTH * 2 + 1];
    hash_password(newPassword, hashedPassword);

    char sql[512];
    snprintf(sql, sizeof(sql), "UPDATE users SET password = '%s' WHERE username = '%s';", hashedPassword, username);

    char* errMsg = NULL;
    rc = sqlite3_exec(DB, sql, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(DB);
        return 0;
    }

    int changes = sqlite3_changes(DB);
    sqlite3_close(DB);
    return (changes > 0) ? 1 : 0;
}

// Verify password hash in database for a username
int verify_password(const char* username, const char* password) {
    sqlite3* DB;
    if (sqlite3_open("users.db", &DB) != SQLITE_OK) {  // نفس القاعدة هنا
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(DB));
        return 0;
    }

    const char* sql = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(DB);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    int result = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* db_hash = sqlite3_column_text(stmt, 0);

        char expected_hash[SHA256_DIGEST_LENGTH * 2 + 1];
        hash_password(password, expected_hash);

        if (strcmp((const char*)db_hash, expected_hash) == 0) {
            result = 1;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return result;
}

// Run a test case and print PASS or FAIL
void run_test_case(const char* username, const char* newPassword) {
    int update_result = updatePassword(username, newPassword);
    int verify_result = verify_password(username, newPassword);

    if (update_result && verify_result) {
        printf("Test case (username='%s', newPassword='%s'): PASS\n", username, newPassword);
    } else {
        printf("Test case (username='%s', newPassword='%s'): FAIL\n", username, newPassword);
    }
}

int main() {
    // نفس التست كيسز بالضبط من البايثون
    run_test_case("user1", "password123");   // user1 موجود → PASS أو FAIL حسب وجوده
    run_test_case("user2", "mypassword");    // user2 موجود → PASS أو FAIL
    run_test_case("user1", "newpassword");   // تحديث user1 → PASS أو FAIL
    run_test_case("user3", "123456");        // user3 موجود → PASS أو FAIL
    run_test_case("", "");                    // اسم مستخدم وباسورد فارغين → FAIL

    return 0;
}
