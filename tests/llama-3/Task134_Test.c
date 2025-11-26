#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/bn.h>

typedef struct {
    RSA* key;
    char name[50]; // store key name
} Key;

typedef struct {
    Key* keys;
    int size;
    int capacity;
} KeyManager;

KeyManager* key_manager_new() {
    KeyManager* manager = (KeyManager*)malloc(sizeof(KeyManager));
    manager->capacity = 4;
    manager->keys = (Key*)malloc(sizeof(Key) * manager->capacity);
    manager->size = 0;
    return manager;
}

void key_manager_free(KeyManager* manager) {
    for (int i = 0; i < manager->size; i++) {
        RSA_free(manager->keys[i].key);
    }
    free(manager->keys);
    free(manager);
}

void key_manager_generate_key(KeyManager* manager, const char* keyName) {
    if (manager->size == manager->capacity) {
        manager->capacity *= 2;
        manager->keys = (Key*)realloc(manager->keys, sizeof(Key) * manager->capacity);
    }
    RSA* key = RSA_new();
    BIGNUM* exponent = BN_new();
    BN_set_word(exponent, 65537);
    if (!RSA_generate_key_ex(key, 2048, exponent, NULL)) {
        fprintf(stderr, "RSA_generate_key_ex failed\n");
        BN_free(exponent);
        return;
    }
    BN_free(exponent);

    manager->keys[manager->size].key = key;
    strncpy(manager->keys[manager->size].name, keyName, sizeof(manager->keys[manager->size].name) - 1);
    manager->keys[manager->size].name[sizeof(manager->keys[manager->size].name)-1] = '\0';
    manager->size++;
}

RSA* key_manager_get_key(KeyManager* manager, const char* keyName) {
    for (int i = 0; i < manager->size; i++) {
        if (strcmp(manager->keys[i].name, keyName) == 0) {
            return manager->keys[i].key;
        }
    }
    return NULL;
}

void key_manager_delete_key(KeyManager* manager, const char* keyName) {
    for (int i = 0; i < manager->size; i++) {
        if (strcmp(manager->keys[i].name, keyName) == 0) {
            RSA_free(manager->keys[i].key);
            // Shift keys left
            for (int j = i; j < manager->size - 1; j++) {
                manager->keys[j] = manager->keys[j + 1];
            }
            manager->size--;
            return;
        }
    }
}

char* key_manager_hash_key(KeyManager* manager, const char* keyName) {
    RSA* key = key_manager_get_key(manager, keyName);
    if (key == NULL) return NULL;

    // Serialize public key DER format
    int len = i2d_RSAPublicKey(key, NULL);
    if (len <= 0) return NULL;
    unsigned char* buf = (unsigned char*)malloc(len);
    unsigned char* p = buf;
    i2d_RSAPublicKey(key, &p);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(buf, len, hash);
    free(buf);

    char* hashString = (char*)malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashString + i * 2, "%02x", hash[i]);
    }
    hashString[SHA256_DIGEST_LENGTH * 2] = '\0';

    return hashString;
}

int main() {
    KeyManager* manager = key_manager_new();
    const char* keyName = "my_key";

    key_manager_generate_key(manager, keyName);
    RSA* key = key_manager_get_key(manager, keyName);

    if (key != NULL) {
        printf("Generated key for '%s': PASS\n", keyName);
    } else {
        printf("Generated key for '%s': FAIL\n", keyName);
        key_manager_free(manager);
        return 1;
    }

    char* hash = key_manager_hash_key(manager, keyName);
    if (hash != NULL) {
        printf("Hash of '%s': %s\n", keyName, hash);
        printf("Hash generation: PASS\n");
        free(hash);
    } else {
        printf("Hash generation: FAIL\n");
    }

    key_manager_delete_key(manager, keyName);
    RSA* deletedKey = key_manager_get_key(manager, keyName);
    if (deletedKey == NULL) {
        printf("Key '%s' deleted successfully: PASS\n", keyName);
    } else {
        printf("Key deletion failed for '%s': FAIL\n", keyName);
    }

    key_manager_free(manager);
    return 0;
}
