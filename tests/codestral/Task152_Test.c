#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE 10000

typedef struct Entry {
    int key;
    int value;
    struct Entry* next;
} Entry;

typedef struct HashTable {
    Entry** table;
} HashTable;

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->table = (Entry**)calloc(SIZE, sizeof(Entry*));
    return ht;
}

int hash(int key) {
    return abs(key) % SIZE;
}

void insert(HashTable* ht, int key, int value) {
    int hashIndex = hash(key);
    Entry* current = ht->table[hashIndex];
    while (current != NULL) {
        if (current->key == key) {
            current->value = value; // overwrite existing
            return;
        }
        current = current->next;
    }
    Entry* newEntry = (Entry*)malloc(sizeof(Entry));
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = ht->table[hashIndex];
    ht->table[hashIndex] = newEntry;
}

void deleteKey(HashTable* ht, int key) {
    int hashIndex = hash(key);
    Entry* current = ht->table[hashIndex];
    Entry* prev = NULL;

    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                ht->table[hashIndex] = current->next;
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

int search(HashTable* ht, int key) {
    int hashIndex = hash(key);
    Entry* current = ht->table[hashIndex];

    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return -1;
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
    free(ht->table);
    free(ht);
}

void runTests() {
    HashTable* ht = createHashTable();

    // Test insert and search
    insert(ht, 1, 100);
    assert(search(ht, 1) == 100);

    // Test insert overwrite
    insert(ht, 1, 200);
    assert(search(ht, 1) == 200);

    // Test search non-existent key
    assert(search(ht, 9999) == -1);

    // Test delete existing key
    insert(ht, 1, 100);
    deleteKey(ht, 1);
    assert(search(ht, 1) == -1);

    // Test delete non-existent key (should do nothing)
    deleteKey(ht, 9999);
    assert(search(ht, 9999) == -1);

    // Test insert multiple keys
    insert(ht, 10, 10);
    insert(ht, 20, 20);
    insert(ht, 30, 30);
    assert(search(ht, 10) == 10);
    assert(search(ht, 20) == 20);
    assert(search(ht, 30) == 30);

    // Test collisions: use keys that collide modulo SIZE
    insert(ht, SIZE + 1, 111);
    insert(ht, 2 * SIZE + 1, 222);
    assert(search(ht, SIZE + 1) == 111);
    assert(search(ht, 2 * SIZE + 1) == 222);

    // Delete collision keys and verify
    deleteKey(ht, SIZE + 1);
    assert(search(ht, SIZE + 1) == -1);
    assert(search(ht, 2 * SIZE + 1) == 222);

    deleteKey(ht, 2 * SIZE + 1);
    assert(search(ht, 2 * SIZE + 1) == -1);

    // Test inserting and deleting many keys
    for (int i = 0; i < 1000; i++) {
        insert(ht, i, i*10);
    }
    for (int i = 0; i < 1000; i++) {
        assert(search(ht, i) == i*10);
    }
    for (int i = 0; i < 1000; i++) {
        deleteKey(ht, i);
        assert(search(ht, i) == -1);
    }

    freeHashTable(ht);

    printf("All test cases passed!\n");
}

int main() {
    runTests();
    return 0;
}
