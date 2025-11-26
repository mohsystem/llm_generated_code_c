#include <stdio.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 50

// قائمة أسماء مستخدمين محاكية لقاعدة بيانات
const char* known_users[] = {"ahmad", "john", "alice", NULL};

int user_exists(const char* username) {
    for (int i = 0; known_users[i] != NULL; i++) {
        if (strcmp(known_users[i], username) == 0) {
            return 1;
        }
    }
    return 0;
}

void get_user_info(const char* username) {
    if (user_exists(username)) {
        printf("User found: %s\n", username);
    } else {
        printf("User not found\n");
    }
}

// دالة اختبار حالات username مختلفة
void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"ahmad", "User found: ahmad\n", "Test Case 1: User exists"},
        {"nonexistent", "User not found\n", "Test Case 2: User does not exist"},
        {"john", "User found: john\n", "Test Case 3: User exists"},
        {"alice", "User found: alice\n", "Test Case 4: User exists"},
        {"bob", "User not found\n", "Test Case 5: User does not exist"},
    };

    char output[100];
    int pass = 1;

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // محاكاة الإخراج في متغير نصي
        if (user_exists(tests[i].input)) {
            snprintf(output, sizeof(output), "User found: %s\n", tests[i].input);
        } else {
            snprintf(output, sizeof(output), "User not found\n");
        }

        printf("Running %s... ", tests[i].description);
        if (strcmp(output, tests[i].expected_output) == 0) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected: %sGot: %s", tests[i].expected_output, output);
            pass = 0;
        }
    }

    if (pass) {
        printf("All tests passed!\n");
    }
}

int main() {
    run_tests();
    return 0;
}
