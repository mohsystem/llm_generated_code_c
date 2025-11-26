#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <sqlite3.h>

void hash_password(const char* password, char* output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[SHA256_DIGEST_LENGTH * 2] = '\0';
}

int update_password(const char* username, const char* new_password) {
    char hashed_password[SHA256_DIGEST_LENGTH * 2 + 1];
    hash_password(new_password, hashed_password);

    sqlite3* db;
    char* err_msg = 0;
    int rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    char sql[512];
    snprintf(sql, sizeof(sql), "UPDATE users SET password = '%s' WHERE username = '%s';", hashed_password, username);

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_close(db);
    return 1;
}

int verify_password(const char* username, const char* password) {
    sqlite3* db;
    int rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char* sql = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
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
    sqlite3_close(db);
    return result;
}

void run_test_case(const char* username, const char* new_password) {
    int update_result = update_password(username, new_password);
    int verify_result = verify_password(username, new_password);
    if (update_result && verify_result) {
        printf("Test case (username='%s', newPassword='%s'): PASS\n", username, new_password);
    } else {
        printf("Test case (username='%s', newPassword='%s'): FAIL\n", username, new_password);
    }
}

int main() {
    // نفس التست كيسز بالضبط من البايثون
    run_test_case("user1", "password123");   // يفترض أن user1 موجود → PASS أو FAIL
    run_test_case("user2", "mypassword");    // يفترض أن user2 موجود → PASS أو FAIL
    run_test_case("user1", "newpassword");   // تحديث user1 → PASS أو FAIL
    run_test_case("user3", "123456");        // يفترض أن user3 موجود → PASS أو FAIL
    run_test_case("", "");                    // اسم مستخدم وباسورد فارغ → FAIL

    return 0;
}
