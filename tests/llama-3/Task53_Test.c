#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char sessionId[37];
    char userId[10];
} Session;

Session sessions[100]; // max 100 sessions
int sessionCount = 0;

void createSession(const char* userId) {
    time_t t;
    time(&t);
    sprintf(sessions[sessionCount].sessionId, "%ld", t);
    strcpy(sessions[sessionCount].userId, userId);
    sessionCount++;
}

const char* getUserId(const char* sessionId) {
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].sessionId, sessionId) == 0) {
            return sessions[i].userId;
        }
    }
    return NULL;
}

void invalidateSession(const char* sessionId) {
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].sessionId, sessionId) == 0) {
            sessions[i].sessionId[0] = '\0';
            sessions[i].userId[0] = '\0';
            return;
        }
    }
}

int main() {
    int pass = 0, fail = 0;

    // 1. test_create_session
    createSession("user123");
    if (sessionCount == 1 && sessions[0].userId[0] != '\0' && sessions[0].sessionId[0] != '\0') {
        printf("Test 1: PASS\n");
        pass++;
    } else {
        printf("Test 1: FAIL\n");
        fail++;
    }

    // 2. test_get_session
    const char* sid = sessions[0].sessionId;
    const char* uid = getUserId(sid);
    if (uid && strcmp(uid, "user123") == 0) {
        printf("Test 2: PASS\n");
        pass++;
    } else {
        printf("Test 2: FAIL\n");
        fail++;
    }

    // 3. test_get_invalid_session
    const char* invalidUser = getUserId("invalid_session");
    if (invalidUser == NULL) {
        printf("Test 3: PASS\n");
        pass++;
    } else {
        printf("Test 3: FAIL\n");
        fail++;
    }

    // 4. test_session_timeout - NOT IMPLEMENTED
    printf("Test 4: FAIL (not implemented)\n");
    fail++;

    // 5. test_destroy_session (invalidateSession)
    invalidateSession(sid);
    const char* uid_after_invalidate = getUserId(sid);
    if (uid_after_invalidate == NULL) {
        printf("Test 5: PASS\n");
        pass++;
    } else {
        printf("Test 5: FAIL\n");
        fail++;
    }

    // 6. test_cleanup_expired_sessions - NOT IMPLEMENTED
    printf("Test 6: FAIL (not implemented)\n");
    fail++;

    // 7. test_cleanup_no_expired_sessions - NOT IMPLEMENTED
    printf("Test 7: FAIL (not implemented)\n");
    fail++;

    // 8. test_update_last_accessed - NOT IMPLEMENTED
    printf("Test 8: FAIL (not implemented)\n");
    fail++;

    // 9. test_create_multiple_sessions
    createSession("user456");
    createSession("user789");
    if (sessionCount >= 3) {
        printf("Test 9: PASS\n");
        pass++;
    } else {
        printf("Test 9: FAIL\n");
        fail++;
    }

    // 10. test_destroy_non_existent_session
    invalidateSession("nonexistent_session");
    // No crash, sessionCount remains unchanged
    if (sessionCount >= 3) {
        printf("Test 10: PASS\n");
        pass++;
    } else {
        printf("Test 10: FAIL\n");
        fail++;
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
