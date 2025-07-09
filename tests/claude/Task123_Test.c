#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_SESSIONS 100
#define SESSION_ID_LENGTH 36
#define USER_ID_LENGTH 20

typedef struct {
    char session_id[SESSION_ID_LENGTH + 1];
    char user_id[USER_ID_LENGTH + 1];
    time_t creation_time;
    time_t last_activity;
} Session;

typedef struct {
    Session sessions[MAX_SESSIONS];
    int count;
    int session_timeout;
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

void create_session(SessionManager *manager, const char *user_id, char *session_id) {
    if (manager->count >= MAX_SESSIONS) {
        printf("Error: Maximum number of sessions reached\n");
        return;
    }

    Session *session = &manager->sessions[manager->count++];
    generate_session_id(session->session_id);
    strncpy(session->user_id, user_id, USER_ID_LENGTH);
    session->creation_time = time(NULL);
    session->last_activity = session->creation_time;

    strcpy(session_id, session->session_id);
}

Session *get_session(SessionManager *manager, const char *session_id) {
    time_t current_time = time(NULL);
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->sessions[i].session_id, session_id) == 0) {
            if (current_time - manager->sessions[i].last_activity > manager->session_timeout) {
                // Session expired
                return NULL;
            }
            manager->sessions[i].last_activity = current_time;
            return &manager->sessions[i];
        }
    }
    return NULL;
}

void destroy_session(SessionManager *manager, const char *session_id) {
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->sessions[i].session_id, session_id) == 0) {
            // Move last session here and decrease count
            manager->sessions[i] = manager->sessions[--manager->count];
            return;
        }
    }
}

void cleanup_expired_sessions(SessionManager *manager) {
    time_t current_time = time(NULL);
    for (int i = 0; i < manager->count; i++) {
        if (current_time - manager->sessions[i].last_activity > manager->session_timeout) {
            destroy_session(manager, manager->sessions[i].session_id);
            i--; // Recheck this index
        }
    }
}

// Helper to find index of session_id
int find_session_index(SessionManager *manager, const char *session_id) {
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->sessions[i].session_id, session_id) == 0) {
            return i;
        }
    }
    return -1;
}

// Check UUID format (basic check)
int is_valid_uuid_format(const char *uuid) {
    if (strlen(uuid) != SESSION_ID_LENGTH) return 0;
    if (uuid[8] != '-' || uuid[13] != '-' || uuid[18] != '-' || uuid[23] != '-') return 0;
    // further checks omitted for brevity
    return 1;
}

// === Test framework ===

#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

void test_create_session_valid_user_id(SessionManager *manager) {
    const char *testname = "test_create_session_valid_user_id";
    char sid[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid);
    int idx = find_session_index(manager, sid);
    int pass = (idx != -1) && (strcmp(manager->sessions[idx].user_id, "user123") == 0);
    ASSERT(pass, testname);
}

void test_create_session_unique_session_id(SessionManager *manager) {
    const char *testname = "test_create_session_unique_session_id";
    char sid1[SESSION_ID_LENGTH+1];
    char sid2[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid1);
    create_session(manager, "user123", sid2);
    ASSERT(strcmp(sid1, sid2) != 0, testname);
}

void test_get_session_valid_session(SessionManager *manager) {
    const char *testname = "test_get_session_valid_session";
    char sid[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid);
    Session *s = get_session(manager, sid);
    ASSERT(s != NULL && strcmp(s->user_id, "user123") == 0, testname);
}

void test_get_session_expired(SessionManager *manager) {
    const char *testname = "test_get_session_expired";
    char sid[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid);
    int idx = find_session_index(manager, sid);
    // artificially expire
    manager->sessions[idx].last_activity -= (manager->session_timeout + 1);
    Session *s = get_session(manager, sid);
    ASSERT(s == NULL, testname);
}

void test_destroy_session(SessionManager *manager) {
    const char *testname = "test_destroy_session";
    char sid[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid);
    destroy_session(manager, sid);
    int idx = find_session_index(manager, sid);
    ASSERT(idx == -1, testname);
}

void test_destroy_nonexistent_session(SessionManager *manager) {
    const char *testname = "test_destroy_nonexistent_session";
    char fake_sid[SESSION_ID_LENGTH+1];
    strcpy(fake_sid, "00000000-0000-0000-0000-000000000000");
    int before = manager->count;
    destroy_session(manager, fake_sid);
    int after = manager->count;
    ASSERT(before == after, testname);
}

void test_cleanup_expired_sessions(SessionManager *manager) {
    const char *testname = "test_cleanup_expired_sessions";
    char sid1[SESSION_ID_LENGTH+1];
    char sid2[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid1);
    create_session(manager, "user456", sid2);
    int idx1 = find_session_index(manager, sid1);
    int idx2 = find_session_index(manager, sid2);
    manager->sessions[idx1].last_activity -= (manager->session_timeout + 1);
    manager->sessions[idx2].last_activity -= (manager->session_timeout + 1);
    cleanup_expired_sessions(manager);
    ASSERT(find_session_index(manager, sid1) == -1 && find_session_index(manager, sid2) == -1, testname);
}

void test_get_session_updates_last_activity(SessionManager *manager) {
    const char *testname = "test_get_session_updates_last_activity";
    char sid[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid);
    int idx = find_session_index(manager, sid);
    time_t old_activity = manager->sessions[idx].last_activity;
    sleep(1);
    get_session(manager, sid);
    time_t new_activity = manager->sessions[find_session_index(manager, sid)].last_activity;
    ASSERT(new_activity > old_activity, testname);
}

void test_create_session_session_id_format(SessionManager *manager) {
    const char *testname = "test_create_session_session_id_format";
    char sid[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid);
    ASSERT(is_valid_uuid_format(sid), testname);
}

void test_no_sessions_after_cleanup(SessionManager *manager) {
    const char *testname = "test_no_sessions_after_cleanup";
    char sid[SESSION_ID_LENGTH+1];
    create_session(manager, "user123", sid);
    int idx = find_session_index(manager, sid);
    manager->sessions[idx].last_activity -= (manager->session_timeout + 1);
    cleanup_expired_sessions(manager);
    ASSERT(manager->count == 0, testname);
}

int main() {
    srand((unsigned int)time(NULL));
    SessionManager manager = {.count = 0, .session_timeout = 1}; // 1 sec timeout for test speed

    test_create_session_valid_user_id(&manager);
    test_create_session_unique_session_id(&manager);
    test_get_session_valid_session(&manager);
    test_get_session_expired(&manager);
    test_destroy_session(&manager);
    test_destroy_nonexistent_session(&manager);
    test_cleanup_expired_sessions(&manager);
    test_get_session_updates_last_activity(&manager);
    test_create_session_session_id_format(&manager);
    test_no_sessions_after_cleanup(&manager);

    printf("All tests completed.\n");
    return 0;
}
