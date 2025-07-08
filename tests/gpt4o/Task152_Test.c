#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SIZE 10

typedef struct {
    int key;
    char value[256];
} HashEntry;

HashEntry* table[SIZE] = {NULL};

int hashFunction(int key) {
    return key % SIZE;
}

void insert(int key, const char* value) {
    int hashKey = hashFunction(key);
    if (table[hashKey] != NULL) {
        free(table[hashKey]);
    }
    table[hashKey] = malloc(sizeof(HashEntry));
    table[hashKey]->key = key;
    strcpy(table[hashKey]->value, value);
}

void deleteKey(int key) {
    int hashKey = hashFunction(key);
    if (table[hashKey] != NULL && table[hashKey]->key == key) {
        free(table[hashKey]);
        table[hashKey] = NULL;
    }
}

const char* search(int key) {
    int hashKey = hashFunction(key);
    if (table[hashKey] != NULL && table[hashKey]->key == key) {
        return table[hashKey]->value;
    }
    return NULL;
}

void freeTable() {
    for (int i = 0; i < SIZE; i++) {
        if (table[i] != NULL) {
            free(table[i]);
            table[i] = NULL;
        }
    }
}

void runTests() {
    // Insert and search basic keys
    insert(1, "one");
    assert(strcmp(search(1), "one") == 0);

    insert(2, "two");
    assert(strcmp(search(2), "two") == 0);

    // Insert a key that collides with key 1 (e.g., 11 % 10 == 1)
    insert(11, "eleven");
    // This overwrites key 1's entry
    assert(search(11) != NULL && strcmp(search(11), "eleven") == 0);
    assert(search(1) == NULL);

    // Delete existing key
    deleteKey(11);
    assert(search(11) == NULL);

    // Delete non-existent key (should be no crash)
    deleteKey(3);
    assert(search(3) == NULL);

    // Reinsert key 1 after deletion of colliding key
    insert(1, "one");
    assert(strcmp(search(1), "one") == 0);

    // Overwrite key 1 again
    insert(1, "ONE");
    assert(strcmp(search(1), "ONE") == 0);

    freeTable();

    printf("All test cases passed!\n");
}

int main() {
    runTests();
    return 0;
}
