#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

void create_table(sqlite3 *db) {
    char *errMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS products ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "name TEXT,"
                      "price REAL);";
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (create_table): %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

void add_product(sqlite3 *db, const char *name, double price) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO products (name, price) VALUES (?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, price);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Execution failed (add_product): %s\n", sqlite3_errmsg(db));
        }
    } else {
        fprintf(stderr, "SQL error (add_product prepare): %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}

void get_product(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM products WHERE id = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            printf("ID: %d, Name: %s, Price: $%.2f\n",
                   sqlite3_column_int(stmt, 0),
                   (const char *)sqlite3_column_text(stmt, 1),
                   sqlite3_column_double(stmt, 2));
            printf("Test case: PASS\n");
        } else {
            printf("Product not found.\n");
            printf("Test case: FAIL\n");
        }
    } else {
        fprintf(stderr, "SQL error (get_product prepare): %s\n", sqlite3_errmsg(db));
        printf("Test case: FAIL\n");
    }
    sqlite3_finalize(stmt);
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("/home/kali/CLionProjects/llm_generated_code_c/tests/gpt4o/Task137_Test/products.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    create_table(db);

    while (1) {
        printf("\n1. Add product\n2. Get product details\n3. Exit\nEnter your choice: ");

        char choice[10];
        if (!fgets(choice, sizeof(choice), stdin)) break;

        if (choice[0] == '1') {
            char name[100];
            double price;

            printf("Enter product name: ");
            if (!fgets(name, sizeof(name), stdin)) break;
            name[strcspn(name, "\n")] = 0; // Remove trailing newline

            printf("Enter product price: ");
            if (scanf("%lf", &price) != 1) {
                printf("Invalid price input\n");
                while(getchar() != '\n'); // flush input
                continue;
            }
            while(getchar() != '\n'); // flush input

            add_product(db, name, price);
            printf("Product added successfully!\n");
        }
        else if (choice[0] == '2') {
            int id;
            printf("Enter product ID: ");
            if (scanf("%d", &id) != 1) {
                printf("Invalid ID input\n");
                while(getchar() != '\n'); // flush input
                continue;
            }
            while(getchar() != '\n'); // flush input

            get_product(db, id);
        }
        else if (choice[0] == '3') {
            break;
        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    sqlite3_close(db);
    return 0;
}
