#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SIZE 11

typedef struct {
    int key;
    char* value;
} HashEntry;

HashEntry* table[SIZE];

int hashFunction(int key) {
    return key % SIZE;
}

int rehash(int oldHash) {
    return (oldHash + 1) % SIZE;
}

void put(int key, const char* value) {
    int hashValue = hashFunction(key);

    if (table[hashValue] == NULL) {
        table[hashValue] = (HashEntry*)malloc(sizeof(HashEntry));
        table[hashValue]->key = key;
        table[hashValue]->value = (char*)malloc(strlen(value) + 1);
        strcpy(table[hashValue]->value, value);
    } else {
        if (table[hashValue]->key == key) {
            free(table[hashValue]->value);
            table[hashValue]->value = (char*)malloc(strlen(value) + 1);
            strcpy(table[hashValue]->value, value); // replace
        } else {
            int nextSlot = rehash(hashValue);
            while (table[nextSlot] != NULL && table[nextSlot]->key != key) {
                nextSlot = rehash(nextSlot);
            }

            if (table[nextSlot] == NULL) {
                table[nextSlot] = (HashEntry*)malloc(sizeof(HashEntry));
                table[nextSlot]->key = key;
                table[nextSlot]->value = (char*)malloc(strlen(value) + 1);
                strcpy(table[nextSlot]->value, value);
            } else {
                free(table[nextSlot]->value);
                table[nextSlot]->value = (char*)malloc(strlen(value) + 1);
                strcpy(table[nextSlot]->value, value); // replace
            }
        }
    }
}

char* get(int key) {
    int startSlot = hashFunction(key);

    int position = startSlot;
    while (table[position] != NULL) {
        if (table[position]->key == key) {
            return table[position]->value;
        } else {
            position = rehash(position);
            if (position == startSlot) {
                break;
            }
        }
    }
    return NULL;
}

void freeTable() {
    for (int i = 0; i < SIZE; i++) {
        if (table[i]) {
            free(table[i]->value);
            free(table[i]);
            table[i] = NULL;
        }
    }
}

void runTests() {
    // Initialize table
    for (int i = 0; i < SIZE; i++) {
        table[i] = NULL;
    }

    // Test insert and search
    put(1, "value1");
    assert(strcmp(get(1), "value1") == 0);

    // Test overwrite existing key
    put(1, "value2");
    assert(strcmp(get(1), "value2") == 0);

    // Test search non-existent key returns NULL
    assert(get(999) == NULL);

    // Test insert multiple keys and search
    put(2, "value2");
    put(3, "value3");
    assert(strcmp(get(2), "value2") == 0);
    assert(strcmp(get(3), "value3") == 0);

    // Test collision handling
    // 12 and 23 both hash to 12%11=1 and 23%11=1, causing collision
    put(12, "val12");
    put(23, "val23");
    assert(strcmp(get(12), "val12") == 0);
    assert(strcmp(get(23), "val23") == 0);

    // Test overwrite after collision
    put(23, "val23_updated");
    assert(strcmp(get(23), "val23_updated") == 0);

    // Test re-insert after deletion (simulate delete by freeing)
    // Not implemented in original code, but you can free manually here
    freeTable();
    for (int i = 0; i < SIZE; i++) {
        table[i] = NULL;
    }
    put(1, "newval1");
    assert(strcmp(get(1), "newval1") == 0);

    printf("All test cases passed!\n");
}

int main() {
    runTests();
    return 0;
}
