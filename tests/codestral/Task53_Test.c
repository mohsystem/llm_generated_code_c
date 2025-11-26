#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// الكود الأصلي كما هو
typedef struct Session {
    char* userId;
    bool isActive;
    struct Session* next;
} Session;

// Hash function
unsigned int hash(const char* str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash % 100;
}

// Global user sessions hash table
Session* userSessions[100] = {NULL};

// Helper to find session by userId in linked list at bucket index
Session* find_session(unsigned int index, const char* userId) {
    Session* curr = userSessions[index];
    while (curr) {
        if (strcmp(curr->userId, userId) == 0 && curr->isActive) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// Create session for userId (adds new active session at front of linked list)
bool create_session(const char* userId) {
    unsigned int index = hash(userId);
    Session* newSession = malloc(sizeof(Session));
    if (!newSession) return false;
    newSession->userId = strdup(userId);
    newSession->isActive = true;
    newSession->next = userSessions[index];
    userSessions[index] = newSession;
    return true;
}

// Destroy session for userId (marks isActive false)
void destroy_session(const char* userId) {
    unsigned int index = hash(userId);
    Session* curr = userSessions[index];
    while (curr) {
        if (strcmp(curr->userId, userId) == 0 && curr->isActive) {
            curr->isActive = false;
            return;
        }
        curr = curr->next;
    }
}

// Test framework mimicking your python test cases
int main() {
    int pass = 0, fail = 0;

    // 1. test_create_session
    if (create_session("user123")) {
        Session* s = find_session(hash("user123"), "user123");
        if (s && s->isActive) {
            printf("Test 1: PASS\n");
            pass++;
        } else {
            printf("Test 1: FAIL\n");
            fail++;
        }
    } else {
        printf("Test 1: FAIL\n");
        fail++;
    }

    // 2. test_get_session
    Session* s2 = find_session(hash("user123"), "user123");
    if (s2 && s2->isActive) {
        printf("Test 2: PASS\n");
        pass++;
    } else {
        printf("Test 2: FAIL\n");
        fail++;
    }

    // 3. test_get_invalid_session
    Session* s3 = find_session(hash("invalidUser"), "invalidUser");
    if (s3 == NULL) {
        printf("Test 3: PASS\n");
        pass++;
    } else {
        printf("Test 3: FAIL\n");
        fail++;
    }

    // 4. test_session_timeout - NOT IMPLEMENTED (no timeout support)
    printf("Test 4: FAIL (not implemented)\n");
    fail++;

    // 5. test_destroy_session
    destroy_session("user123");
    Session* s5 = find_session(hash("user123"), "user123");
    if (s5 == NULL) {
        printf("Test 5: PASS\n");
        pass++;
    } else {
        printf("Test 5: FAIL\n");
        fail++;
    }

    // 6. test_cleanup_expired_sessions - NOT IMPLEMENTED (no expiration or cleanup)
    printf("Test 6: FAIL (not implemented)\n");
    fail++;

    // 7. test_cleanup_no_expired_sessions - NOT IMPLEMENTED
    printf("Test 7: FAIL (not implemented)\n");
    fail++;

    // 8. test_update_last_accessed - NOT IMPLEMENTED (no timestamps stored)
    printf("Test 8: FAIL (not implemented)\n");
    fail++;

    // 9. test_create_multiple_sessions
    create_session("user456");
    Session* s9a = find_session(hash("user123"), "user123");
    Session* s9b = find_session(hash("user456"), "user456");
    if ((s9a == NULL || s9a->isActive == false) && s9b && s9b->isActive) {
        // user123 session destroyed previously, so it can be inactive
        printf("Test 9: PASS\n");
        pass++;
    } else {
        printf("Test 9: FAIL\n");
        fail++;
    }

    // 10. test_destroy_non_existent_session
    destroy_session("nonExistentUser");
    // Should not crash or affect others
    Session* s10 = find_session(hash("user456"), "user456");
    if (s10 && s10->isActive) {
        printf("Test 10: PASS\n");
        pass++;
    } else {
        printf("Test 10: FAIL\n");
        fail++;
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    // Free allocated memory to be clean (not mandatory here)
    for (int i=0; i<100; i++) {
        Session* curr = userSessions[i];
        while(curr) {
            Session* next = curr->next;
            free(curr->userId);
            free(curr);
            curr = next;
        }
        userSessions[i] = NULL;
    }

    return 0;
}
