// Note: This code strictly uses the provided C example with libmongoc as is,
// and simply prints PASS or FAIL for each test case from the Python prompt,
// because full MongoDB interaction testing is environment-dependent and cannot
// be simulated fully here without a running MongoDB instance.

// This example assumes libmongoc is installed and linked properly.
// Compile with: gcc -o mongo_test mongo_test.c $(pkg-config --cflags --libs libmongoc-1.0)

#include <mongoc/mongoc.h>
#include <stdio.h>
#include <string.h>

int main() {
    mongoc_client_t *client;
    mongoc_database_t *database;
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc;
    char *str;

    mongoc_init();
    client = mongoc_client_new("mongodb://localhost:27017");
    database = mongoc_client_get_database(client, "test-database");
    collection = mongoc_client_get_collection(client, "test-database", "test-collection");

    // Test case 1: Create document {"name": "John", "age": 30}
    doc = BCON_NEW("name", BCON_UTF8("John"), "age", BCON_INT32(30));
    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "Test case 1 (Create): FAIL - %s\n", error.message);
    } else {
        printf("Test case 1 (Create): PASS\n");
    }
    bson_destroy(doc);

    // Test case 2: Read document {"name": "John"}
    doc = BCON_NEW("name", BCON_UTF8("John"));
    bson_t *result = mongoc_collection_find_one(collection, doc, NULL, NULL, NULL, &error);
    if (!result) {
        fprintf(stderr, "Test case 2 (Read): FAIL - %s\n", error.message);
    } else {
        str = bson_as_json(result, NULL);
        printf("Test case 2 (Read): PASS - %s\n", str);
        bson_free(str);
        bson_destroy(result);
    }
    bson_destroy(doc);

    // Test case 3: Update document {"name": "John"} set age=31
    doc = BCON_NEW("name", BCON_UTF8("John"));
    bson_t *update = BCON_NEW("$set", "{", "age", BCON_INT32(31), "}");
    if (!mongoc_collection_update_one(collection, doc, update, NULL, NULL, &error)) {
        fprintf(stderr, "Test case 3 (Update): FAIL - %s\n", error.message);
    } else {
        printf("Test case 3 (Update): PASS\n");
    }
    bson_destroy(doc);
    bson_destroy(update);

    // Test case 4: Delete document {"name": "John"}
    doc = BCON_NEW("name", BCON_UTF8("John"));
    if (!mongoc_collection_delete_one(collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "Test case 4 (Delete): FAIL - %s\n", error.message);
    } else {
        printf("Test case 4 (Delete): PASS\n");
    }
    bson_destroy(doc);

    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return 0;
}
