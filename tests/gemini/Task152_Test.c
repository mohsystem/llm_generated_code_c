#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TABLE_SIZE 10

typedef struct Entry {
    char *key;
    char *value;
    struct Entry *next;
} Entry;

typedef struct HashTable {
    Entry **table;
    int size;
} HashTable;

HashTable *createHashTable(int size) {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    ht->size = size;
    ht->table = (Entry **)calloc(size, sizeof(Entry *));
    return ht;
}

unsigned int hash(const char *key) {
    unsigned int hashVal = 0;
    while (*key != '\0') {
        hashVal = (hashVal << 5) + *key++;
    }
    return hashVal;
}

void insert(HashTable *ht, const char *key, const char *value) {
    unsigned int index = hash(key) % ht->size;
    Entry *entry = ht->table[index];
    // Check for overwrite if key exists
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = strdup(value);
            return;
        }
        entry = entry->next;
    }
    // Insert new entry
    Entry *newEntry = (Entry *)malloc(sizeof(Entry));
    newEntry->key = strdup(key);
    newEntry->value = strdup(value);
    newEntry->next = ht->table[index];
    ht->table[index] = newEntry;
}

char *search(HashTable *ht, const char *key) {
    unsigned int index = hash(key) % ht->size;
    Entry *entry = ht->table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void deleteItem(HashTable *ht, const char *key) {
    unsigned int index = hash(key) % ht->size;
    Entry *prev = NULL;
    Entry *entry = ht->table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prev == NULL) {
                ht->table[index] = entry->next;
            } else {
                prev->next = entry->next;
            }
            free(entry->key);
            free(entry->value);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void freeHashTable(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        Entry *entry = ht->table[i];
        while (entry != NULL) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

void runTests() {
    HashTable *ht = createHashTable(TABLE_SIZE);

    // Test insert and search
    insert(ht, "key1", "value1");
    assert(strcmp(search(ht, "key1"), "value1") == 0);

    // Test insert overwrite
    insert(ht, "key1", "value2");
    assert(strcmp(search(ht, "key1"), "value2") == 0);

    // Test search non-existent key
    assert(search(ht, "nonexistent_key") == NULL);

    // Test delete existing key
    insert(ht, "key1", "value1");
    deleteItem(ht, "key1");
    assert(search(ht, "key1") == NULL);

    // Test delete non-existent key (should not crash)
    deleteItem(ht, "nonexistent_key");
    assert(search(ht, "nonexistent_key") == NULL);

    // Test insert multiple keys
    insert(ht, "key1", "value1");
    insert(ht, "key2", "value2");
    insert(ht, "key3", "value3");
    assert(strcmp(search(ht, "key1"), "value1") == 0);
    assert(strcmp(search(ht, "key2"), "value2") == 0);
    assert(strcmp(search(ht, "key3"), "value3") == 0);

    // Test collision handling by forcing keys to same bucket (simple but effective test)
    // Using keys with same hash index for the given table size is complex, but for testing,
    // just insert multiple keys, collisions handled internally.
    insert(ht, "a", "1");
    insert(ht, "k", "2"); // likely to collide with 'a' in a small table size
    assert(strcmp(search(ht, "a"), "1") == 0);
    assert(strcmp(search(ht, "k"), "2") == 0);

    // Test delete key from bucket with multiple entries (collision)
    deleteItem(ht, "a");
    assert(search(ht, "a") == NULL);
    assert(strcmp(search(ht, "k"), "2") == 0);

    // Test delete all keys
    const char* keys[] = {"key1", "key2", "key3", "k"};
    for (int i = 0; i < 4; i++) {
        deleteItem(ht, keys[i]);
        assert(search(ht, keys[i]) == NULL);
    }

    // Test reinsert after delete
    insert(ht, "key1", "value1");
    deleteItem(ht, "key1");
    insert(ht, "key1", "value2");
    assert(strcmp(search(ht, "key1"), "value2") == 0);

    freeHashTable(ht);

    printf("All test cases passed!\n");
}

int main() {
    runTests();
    return 0;
}
