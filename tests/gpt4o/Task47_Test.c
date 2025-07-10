#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_ITEM_LENGTH 500

// إنشاء جدول scraped_data
int create_table(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS scraped_data (content TEXT);";
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (create_table): %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

// تخزين البيانات في قاعدة البيانات (كود مشابه storeData)
int store_in_database(sqlite3 *db, char data[MAX_ITEMS][MAX_ITEM_LENGTH], int count) {
    const char *sql = "INSERT INTO scraped_data (content) VALUES (?);";
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

// اختبار store_in_database ويتحقق من البيانات المخزنة
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

    // بيانات اختبار ثابتة - نفس بيانات اختبار البايثون
    char data[3][MAX_ITEM_LENGTH] = {
        "Paragraph 1",
        "Paragraph 2",
        "Paragraph 3"
    };

    if (!store_in_database(db, data, 3)) {
        sqlite3_close(db);
        return 0;
    }

    // استرجاع البيانات والتحقق
    sqlite3_stmt *stmt;
    const char *sql = "SELECT content FROM scraped_data ORDER BY rowid;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    int index = 0;
    int pass = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (strcmp((const char *)text, data[index]) != 0) {
            pass = 0;
            break;
        }
        index++;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (pass && index == 3);
}

// صعوبة محاكاة scrapeData بسبب الاعتماد على الشبكة وgumbo
int test_scrape_data() {
    return 0; // Not implemented
}

int main() {
    printf("Test scrape_website: %s\n", test_scrape_data() ? "PASS" : "FAIL (not implemented)\n");
    printf("Test store_in_database: %s\n", test_store_in_database() ? "PASS" : "FAIL\n");
    return 0;
}
