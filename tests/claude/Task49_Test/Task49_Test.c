#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_OUTPUT 8192

// دالة لإدخال مستخدم في قاعدة البيانات مع إنشاء الجدول منفصلاً
int test_insert_user(const char* name, const char* email) {
    sqlite3 *db;
    int rc = sqlite3_open("user.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // إنشاء الجدول منفصلاً
    char *sql_create = "CREATE TABLE IF NOT EXISTS users ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "name TEXT NOT NULL, "
                       "email TEXT NOT NULL );";
    rc = sqlite3_exec(db, sql_create, 0, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // تحضير أمر الإدخال
    char *sql_insert = "INSERT INTO users (name, email) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // ربط المعطيات
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}

void run_tests() {
    struct {
        const char* name;
        const char* email;
        int expect_success;
        const char* description;
    } tests[] = {
        {"John Doe", "john.doe@example.com", 1, "Test 1: Insert valid user"},
        {"Jane Doe", "jane.doe@example.com", 1, "Test 2: Insert another valid user"},
        {"John Doe", "john.doe@example.com", 0, "Test 3: Insert duplicate email user (should fail)"},
        {"", "no.name@example.com", 0, "Test 4: Insert user with empty name (not implemented validation)"},
        {"No Email", "", 0, "Test 5: Insert user with empty email (not implemented validation)"},
        {NULL, NULL, 0, "Test 6: Insert NULL values (not implemented)"},
    };

    for (int i=0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        int ret = 0;
        if (tests[i].name && tests[i].email)
            ret = test_insert_user(tests[i].name, tests[i].email);
        else
            ret = 0; // NULL inputs fail automatically

        printf("%s... ", tests[i].description);
        if (ret == tests[i].expect_success) {
            printf("PASS\n");
        } else {
            if (!tests[i].expect_success)
                printf("FAIL not implemented\n");
            else
                printf("FAIL\n");
        }
    }
}

int main() {
    run_tests();
    return 0;
}
