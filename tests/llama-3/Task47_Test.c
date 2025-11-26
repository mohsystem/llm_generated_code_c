#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

#define MAX_ITEMS 3
#define MAX_ITEM_LENGTH 1000

// إنشاء جدول data
int create_table(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS data (id INTEGER PRIMARY KEY, data TEXT);";
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (create_table): %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

// تخزين بيانات في قاعدة البيانات
int store_in_database(sqlite3 *db, char data[MAX_ITEMS][MAX_ITEM_LENGTH], int count) {
    const char *sql = "INSERT INTO data (data) VALUES (?);";
    int rc;

    for (int i = 0; i < count; i++) {
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error (prepare): %s\n", sqlite3_errmsg(db));
            return 0;
        }
        sqlite3_bind_text(stmt, 1, data[i], -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error (step): %s\n", sqlite3_errmsg(db));
            return 0;
        }
    }
    return 1;
}

// اختبار تخزين البيانات والتحقق من محتوى الجدول
int test_store_in_database() {
    sqlite3 *db;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open in-memory database\n");
        return 0;
    }
    if (create_table(db) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    char data[MAX_ITEMS][MAX_ITEM_LENGTH] = {
        "Paragraph 1",
        "Paragraph 2",
        "Paragraph 3"
    };

    if (!store_in_database(db, data, MAX_ITEMS)) {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT data FROM data ORDER BY id;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    int index = 0;
    int pass = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (strcmp((const char*)text, data[index]) != 0) {
            pass = 0;
            break;
        }
        index++;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (pass && index == MAX_ITEMS);
}

// اختبار scrapeWebsite (غير مدعوم)
int test_scrape_website() {
    return 0;  // not implemented
}

int main() {
    printf("Test scrape_website: %s\n", test_scrape_website() ? "PASS" : "FAIL (not implemented)\n");
    printf("Test store_in_database: %s\n", test_store_in_database() ? "PASS" : "FAIL\n");
    return 0;
}
