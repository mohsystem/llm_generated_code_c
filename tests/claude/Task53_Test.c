#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SESSIONS 100
#define SESSION_ID_LENGTH 36
#define USER_ID_LENGTH 20

typedef struct {
    char session_id[SESSION_ID_LENGTH + 1];
    char user_id[USER_ID_LENGTH + 1];
    time_t created_at;
    time_t last_accessed;
} Session;

typedef struct {
    Session sessions[MAX_SESSIONS];
    int count;
    time_t session_timeout;
} SessionManager;

void generate_session_id(char *session_id) {
    const char *chars = "0123456789abcdef";
    for (int i = 0; i < SESSION_ID_LENGTH; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            session_id[i] = '-';
        } else {
            session_id[i] = chars[rand() % 16];
        }
    }
    session_id[SESSION_ID_LENGTH] = '\0';
}

void init_session_manager(SessionManager *manager, time_t timeout) {
    manager->count = 0;
    manager->session_timeout = timeout;
}

char* create_session(SessionManager *manager, const char *user_id) {
    if (manager->count >= MAX_SESSIONS) {
        return NULL;
    }

    Session *session = &manager->sessions[manager->count++];
    generate_session_id(session->session_id);
    strncpy(session->user_id, user_id, USER_ID_LENGTH);
    session->user_id[USER_ID_LENGTH] = '\0';
    session->created_at = time(NULL);
    session->last_accessed = session->created_at;

    return session->session_id;
}

Session* get_session(SessionManager *manager, const char *session_id) {
    time_t now = time(NULL);
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->sessions[i].session_id, session_id) == 0) {
            if (now - manager->sessions[i].last_accessed > manager->session_timeout) {
                // Session expired
                memmove(&manager->sessions[i], &manager->sessions[i + 1],
                        (manager->count - i - 1) * sizeof(Session));
                manager->count--;
                return NULL;
            }
            manager->sessions[i].last_accessed = now;
            return &manager->sessions[i];
        }
    }
    return NULL;
}

void destroy_session(SessionManager *manager, const char *session_id) {
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->sessions[i].session_id, session_id) == 0) {
            memmove(&manager->sessions[i], &manager->sessions[i + 1],
                    (manager->count - i - 1) * sizeof(Session));
            manager->count--;
            return;
        }
    }
}

void cleanup_expired_sessions(SessionManager *manager) {
    time_t now = time(NULL);
    int i = 0;
    while (i < manager->count) {
        if (now - manager->sessions[i].last_accessed > manager->session_timeout) {
            memmove(&manager->sessions[i], &manager->sessions[i + 1],
                    (manager->count - i - 1) * sizeof(Session));
            manager->count--;
        } else {
            i++;
        }
    }
}

// Helper function to compare strings safely
int str_equal(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

// Helper to simulate sleep in seconds (cross-platform)
void wait_seconds(int seconds) {
    time_t start = time(NULL);
    while(time(NULL) - start < seconds);
}

int main() {
    srand(time(NULL)); // Seed rand for session id generation

    int tests_passed = 0;
    int tests_failed = 0;

    // 1. Initialize
    SessionManager manager;
    init_session_manager(&manager, 1800); // default 30 min timeout

    // Test data
    const char *user_id = "user123";
    char *session_id = create_session(&manager, user_id);

    // Test case 1: test_create_session
    if (session_id != NULL) {
        int found = 0;
        for (int i=0; i<manager.count; i++) {
            if (str_equal(manager.sessions[i].session_id, session_id)) {
                found = 1;
                if (!str_equal(manager.sessions[i].user_id, user_id)) {
                    printf("Test 1: FAIL (user_id mismatch)\n");
                    tests_failed++;
                    goto after_test1;
                }
                if (strlen(manager.sessions[i].session_id) != SESSION_ID_LENGTH) {
                    printf("Test 1: FAIL (session_id length incorrect)\n");
                    tests_failed++;
                    goto after_test1;
                }
                break;
            }
        }
        if (found) {
            printf("Test 1: PASS\n");
            tests_passed++;
        } else {
            printf("Test 1: FAIL (session not found in manager)\n");
            tests_failed++;
        }
    } else {
        printf("Test 1: FAIL (session_id NULL)\n");
        tests_failed++;
    }
after_test1:

    // Test case 2: test_get_session (valid session)
    Session *sess = get_session(&manager, session_id);
    if (sess != NULL && str_equal(sess->user_id, user_id)) {
        printf("Test 2: PASS\n");
        tests_passed++;
    } else {
        printf("Test 2: FAIL\n");
        tests_failed++;
    }

    // Test case 3: test_get_invalid_session
    char invalid_session[SESSION_ID_LENGTH + 1];
    generate_session_id(invalid_session); // random new ID
    sess = get_session(&manager, invalid_session);
    if (sess == NULL) {
        printf("Test 3: PASS\n");
        tests_passed++;
    } else {
        printf("Test 3: FAIL\n");
        tests_failed++;
    }

    // Test case 4: test_session_timeout
    manager.session_timeout = 1; // 1 second timeout for test
    wait_seconds(2); // wait 2 seconds to expire session
    sess = get_session(&manager, session_id);
    if (sess == NULL) {
        printf("Test 4: PASS\n");
        tests_passed++;
    } else {
        printf("Test 4: FAIL\n");
        tests_failed++;
    }

    // Reset for next tests
    init_session_manager(&manager, 1800);
    session_id = create_session(&manager, user_id);

    // Test case 5: test_destroy_session
    destroy_session(&manager, session_id);
    int still_exists = 0;
    for (int i=0; i<manager.count; i++) {
        if (str_equal(manager.sessions[i].session_id, session_id)) {
            still_exists = 1;
            break;
        }
    }
    if (!still_exists) {
        printf("Test 5: PASS\n");
        tests_passed++;
    } else {
        printf("Test 5: FAIL\n");
        tests_failed++;
    }

    // Test case 6: test_cleanup_expired_sessions
    init_session_manager(&manager, 1); // 1 second timeout
    session_id = create_session(&manager, user_id);
    char *session_id2 = create_session(&manager, "user456");
    wait_seconds(2);
    cleanup_expired_sessions(&manager);
    int found_s1 = 0, found_s2 = 0;
    for (int i=0; i<manager.count; i++) {
        if (str_equal(manager.sessions[i].session_id, session_id)) found_s1=1;
        if (str_equal(manager.sessions[i].session_id, session_id2)) found_s2=1;
    }
    if (!found_s1 && found_s2) {
        printf("Test 6: PASS\n");
        tests_passed++;
    } else {
        printf("Test 6: FAIL\n");
        tests_failed++;
    }

    // Test case 7: test_cleanup_no_expired_sessions
    init_session_manager(&manager, 1800);
    session_id = create_session(&manager, user_id);
    session_id2 = create_session(&manager, "user789");
    cleanup_expired_sessions(&manager);
    found_s1 = 0; found_s2 = 0;
    for (int i=0; i<manager.count; i++) {
        if (str_equal(manager.sessions[i].session_id, session_id)) found_s1=1;
        if (str_equal(manager.sessions[i].session_id, session_id2)) found_s2=1;
    }
    if (found_s1 && found_s2) {
        printf("Test 7: PASS\n");
        tests_passed++;
    } else {
        printf("Test 7: FAIL\n");
        tests_failed++;
    }

    // Test case 8: test_update_last_accessed
    init_session_manager(&manager, 1800);
    session_id = create_session(&manager, user_id);
    // Wait 1 second to allow last_accessed to change
    wait_seconds(1);
    Session *s = get_session(&manager, session_id);
    if (s != NULL && s->last_accessed > s->created_at) {
        printf("Test 8: PASS\n");
        tests_passed++;
    } else {
        printf("Test 8: FAIL\n");
        tests_failed++;
    }

    // Test case 9: test_create_multiple_sessions
    init_session_manager(&manager, 1800);
    session_id = create_session(&manager, user_id);
    session_id2 = create_session(&manager, "user456");
    int found1 = 0, found2 = 0;
    for (int i=0; i<manager.count; i++) {
        if (str_equal(manager.sessions[i].session_id, session_id)) found1 = 1;
        if (str_equal(manager.sessions[i].session_id, session_id2)) found2 = 1;
    }
    if (found1 && found2 && !str_equal(session_id, session_id2)) {
        printf("Test 9: PASS\n");
        tests_passed++;
    } else {
        printf("Test 9: FAIL\n");
        tests_failed++;
    }

    // Test case 10: test_destroy_non_existent_session
    init_session_manager(&manager, 1800);
    session_id = create_session(&manager, user_id);
    char non_exist_session[SESSION_ID_LENGTH + 1];
    generate_session_id(non_exist_session);
    destroy_session(&manager, non_exist_session);
    // session manager count should not change
    if (manager.count == 1 && str_equal(manager.sessions[0].session_id, session_id)) {
        printf("Test 10: PASS\n");
        tests_passed++;
    } else {
        printf("Test 10: FAIL\n");
        tests_failed++;
    }

    // Summary
    printf("\nTotal Passed: %d\nTotal Failed: %d\n", tests_passed, tests_failed);

    return 0;
}
