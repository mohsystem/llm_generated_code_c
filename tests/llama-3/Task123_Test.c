#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int userId;
    char username[256];
} Session;

typedef struct {
    Session* sessions;
    int size;
} SessionManager;

SessionManager* createSessionManager() {
    SessionManager* manager = (SessionManager*)malloc(sizeof(SessionManager));
    manager->sessions = (Session*)malloc(sizeof(Session) * 10);
    manager->size = 0;
    return manager;
}

void createSession(SessionManager* manager, int userId, const char* username) {
    if (manager->size < 10) {
        manager->sessions[manager->size].userId = userId;
        strcpy(manager->sessions[manager->size].username, username);
        manager->size++;
        printf("Session created for user %s with ID %d\n", username, userId);
    } else {
        printf("Session manager is full\n");
    }
}

const char* getUsername(SessionManager* manager, int userId) {
    for (int i = 0; i < manager->size; i++) {
        if (manager->sessions[i].userId == userId) {
            return manager->sessions[i].username;
        }
    }
    return NULL;
}

void deleteSession(SessionManager* manager, int userId) {
    for (int i = 0; i < manager->size; i++) {
        if (manager->sessions[i].userId == userId) {
            for (int j = i; j < manager->size - 1; j++) {
                manager->sessions[j] = manager->sessions[j + 1];
            }
            manager->size--;
            printf("Session deleted for user ID %d\n", userId);
            return;
        }
    }
    printf("No session found for user ID %d\n", userId);
}

// === Test framework ===

#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

int sessionIdExists(SessionManager* manager, int userId) {
    for (int i = 0; i < manager->size; i++) {
        if (manager->sessions[i].userId == userId) {
            return 1;
        }
    }
    return 0;
}

void test_create_session_valid_user_id(SessionManager* manager) {
    const char* testname = "test_create_session_valid_user_id";
    int userId = 123;
    createSession(manager, userId, "testuser");
    ASSERT(sessionIdExists(manager, userId), testname);
}

void test_create_session_unique_session_id(SessionManager* manager) {
    const char* testname = "test_create_session_unique_session_id";
    createSession(manager, 1, "UserA");
    createSession(manager, 2, "UserB");
    int unique = (manager->sessions[0].userId != manager->sessions[1].userId);
    ASSERT(unique, testname);
}

void test_get_session_valid_session(SessionManager* manager) {
    const char* testname = "test_get_session_valid_session";
    createSession(manager, 10, "Alice");
    const char* username = getUsername(manager, 10);
    ASSERT(username != NULL && strcmp(username, "Alice") == 0, testname);
}

void test_get_session_expired(SessionManager* manager) {
    // لا يوجد دعم لانتهاء الجلسة بالوقت في الكود الحالي
    printf("test_get_session_expired: FAIL (Not Implemented)\n");
}

void test_destroy_session(SessionManager* manager) {
    const char* testname = "test_destroy_session";
    createSession(manager, 50, "Bob");
    deleteSession(manager, 50);
    ASSERT(!sessionIdExists(manager, 50), testname);
}

void test_destroy_nonexistent_session(SessionManager* manager) {
    const char* testname = "test_destroy_nonexistent_session";
    int before = manager->size;
    deleteSession(manager, 9999);  // حذف جلسة غير موجودة
    int after = manager->size;
    ASSERT(before == after, testname);
}

void test_cleanup_expired_sessions(SessionManager* manager) {
    // لا يوجد دعم لتنظيف الجلسات المنتهية في الكود الحالي
    printf("test_cleanup_expired_sessions: FAIL (Not Implemented)\n");
}

void test_get_session_updates_last_activity(SessionManager* manager) {
    // لا يوجد last_activity في الكود الحالي => لا يمكن اختبار
    printf("test_get_session_updates_last_activity: FAIL (Not Implemented)\n");
}

void test_create_session_session_id_format(SessionManager* manager) {
    // لا يوجد session ID UUID في الكود الحالي
    printf("test_create_session_session_id_format: FAIL (Not Implemented)\n");
}

void test_no_sessions_after_cleanup(SessionManager* manager) {
    // لا يوجد تنظيف جلسات
    printf("test_no_sessions_after_cleanup: FAIL (Not Implemented)\n");
}

int main() {
    SessionManager* manager = createSessionManager();

    test_create_session_valid_user_id(manager);
    test_create_session_unique_session_id(manager);
    test_get_session_valid_session(manager);
    test_get_session_expired(manager);
    test_destroy_session(manager);
    test_destroy_nonexistent_session(manager);
    test_cleanup_expired_sessions(manager);
    test_get_session_updates_last_activity(manager);
    test_create_session_session_id_format(manager);
    test_no_sessions_after_cleanup(manager);

    // تحرّر الذاكرة
    free(manager->sessions);
    free(manager);

    return 0;
}
