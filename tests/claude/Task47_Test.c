#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

#define MAX_ITEMS 1000
#define MAX_ITEM_LENGTH 1000

// إنشاء جدول scraped_text
int create_table(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS scraped_text(id INTEGER PRIMARY KEY, content TEXT);";
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
    const char *sql = "INSERT INTO scraped_text (content) VALUES (?);";
    int rc;
    char *errMsg = 0;

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

// اختبار تخزين البيانات (تحقق من نفس البيانات)
int test_store_in_database() {
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open in-memory database\n");
        return 0;
    }

    if (create_table(db) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // بيانات الاختبار من البايثون
    char data[3][MAX_ITEM_LENGTH] = {
        "Paragraph 1",
        "Paragraph 2",
        "Paragraph 3"
    };

    int count = 3;

    if (!store_in_database(db, data, count)) {
        sqlite3_close(db);
        return 0;
    }

    // استعلام لاسترداد البيانات والتأكد منها
    sqlite3_stmt *stmt;
    const char *sql = "SELECT content FROM scraped_text ORDER BY id;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    int index = 0;
    int pass = 1;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (strcmp((const char*)text, data[index]) != 0) {
            pass = 0;
            break;
        }
        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (pass && index == count);
}

// بما أن وظيفة scrapeWebsite تعتمد على curl و HTML parsing ولا يمكن محاكاتها هنا بدقة بدون مكتبات معقدة
// سنطبع FAIL للاختبار scrape_website كما في البايثون (mocked in python)
// لأنه غير ممكن تنفيذه في C بدون مكتبات خارجية أو اختبار معقد
int test_scrape_website() {
    // Not implemented due to complexity
    return 0;
}

int main() {
    printf("Test scrape_website: %s\n", test_scrape_website() ? "PASS" : "FAIL (not implemented)\n");
    printf("Test store_in_database: %s\n", test_store_in_database() ? "PASS" : "FAIL\n");
    return 0;
}
