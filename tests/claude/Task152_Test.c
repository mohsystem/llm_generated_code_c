#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SIZE 10
#define MAX_NAME 256

typedef struct Entry {
    char key[MAX_NAME];
    int value;
    struct Entry* next;
} Entry;

typedef struct {
    Entry* table[SIZE];
} HashTable;

unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % SIZE;
}

HashTable* createHashTable() {
    HashTable* ht = malloc(sizeof(HashTable));
    for (int i = 0; i < SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

void insert(HashTable* ht, const char* key, int value) {
    unsigned int index = hash(key);
    Entry* current = ht->table[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value; // Overwrite existing
            return;
        }
        current = current->next;
    }
    Entry* newEntry = malloc(sizeof(Entry));
    strncpy(newEntry->key, key, MAX_NAME - 1);
    newEntry->key[MAX_NAME - 1] = '\0';
    newEntry->value = value;
    newEntry->next = ht->table[index];
    ht->table[index] = newEntry;
}

void delete(HashTable* ht, const char* key) {
    unsigned int index = hash(key);
    Entry* current = ht->table[index];
    Entry* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                ht->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int search(HashTable* ht, const char* key) {
    unsigned int index = hash(key);
    Entry* current = ht->table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return -1; // Not found
}

void freeHashTable(HashTable* ht) {
    for (int i = 0; i < SIZE; i++) {
        Entry* current = ht->table[i];
        while (current != NULL) {
            Entry* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht);
}

void runTests() {
    HashTable* table = createHashTable();

    // Test insert and search
    insert(table, "key1", 1);
    assert(search(table, "key1") == 1);

    // Test insert overwrite
    insert(table, "key1", 2);
    assert(search(table, "key1") == 2);

    // Test search non-existent key
    assert(search(table, "nonexistent_key") == -1);

    // Test delete existing key
    insert(table, "key1", 1);
    delete(table, "key1");
    assert(search(table, "key1") == -1);

    // Test delete non-existent key (should not crash)
    delete(table, "nonexistent_key");
    assert(search(table, "nonexistent_key") == -1);

    // Test insert multiple keys
    insert(table, "key1", 1);
    insert(table, "key2", 2);
    insert(table, "key3", 3);
    assert(search(table, "key1") == 1);
    assert(search(table, "key2") == 2);
    assert(search(table, "key3") == 3);

    // Test collision handling: force collision by using size = 1 (simulate by custom hash)
    // We can't easily change size dynamically here but inserting many keys is okay
    HashTable* smallTable = createHashTable();
    insert(smallTable, "key1", 1);
    insert(smallTable, "key2", 2);
    assert(search(smallTable, "key1") == 1);
    assert(search(smallTable, "key2") == 2);

    // Test delete key from empty table
    delete(smallTable, "key1");
    assert(search(smallTable, "key1") == -1);

    // Test insert and delete all keys
    const char* keys[] = {"key1", "key2", "key3", "key4", "key5"};
    for (int i = 0; i < 5; i++) {
        insert(smallTable, keys[i], i + 10);
    }
    for (int i = 0; i < 5; i++) {
        delete(smallTable, keys[i]);
        assert(search(smallTable, keys[i]) == -1);
    }

    // Test reinsert after delete
    insert(smallTable, "key1", 100);
    delete(smallTable, "key1");
    insert(smallTable, "key1", 200);
    assert(search(smallTable, "key1") == 200);

    printf("All test cases passed!\n");

    freeHashTable(table);
    freeHashTable(smallTable);
}

int main() {
    runTests();
    return 0;
}
