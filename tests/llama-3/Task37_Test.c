#include <stdio.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 256

// قائمة أسماء مستخدمين محاكاة
const char* known_users[] = {"ahmad", "john", "alice", NULL};

// دالة محاكاة البحث في قاعدة البيانات
int user_exists(const char* username) {
    for (int i = 0; known_users[i] != NULL; i++) {
        if (strcmp(known_users[i], username) == 0) {
            return 1;
        }
    }
    return 0;
}

// دالة محاكاة طباعة النتائج
void get_user_info_sim(const char* username, char* output, size_t max_output_size) {
    if (user_exists(username)) {
        snprintf(output, max_output_size, "User found: %s\n", username);
    } else {
        snprintf(output, max_output_size, "User not found\n");
    }
}

// دالة اختبار
void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"ahmad", "User found: ahmad\n", "Test Case 1: User exists"},
        {"nonexistent", "User not found\n", "Test Case 2: User not exists"},
        {"john", "User found: john\n", "Test Case 3: User exists"},
        {"alice", "User found: alice\n", "Test Case 4: User exists"},
        {"bob", "User not found\n", "Test Case 5: User not exists"},
    };

    char output[MAX_OUTPUT_SIZE];
    int pass = 1;

    for (int i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) {
        get_user_info_sim(tests[i].input, output, sizeof(output));
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
