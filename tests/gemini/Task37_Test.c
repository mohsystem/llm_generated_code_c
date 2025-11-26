#include <stdio.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1000

// بيانات مستخدمين محاكية
const char* known_users[] = {"ahmad", "john", "alice", NULL};

// محاكاة دالة callback في sqlite3_exec
int callback_sim(const char* username, char* output, size_t max_output_size) {
    for (int i = 0; known_users[i] != NULL; i++) {
        if (strcmp(known_users[i], username) == 0) {
            snprintf(output, max_output_size, "username = %s\n\n", username);
            return 0;
        }
    }
    snprintf(output, max_output_size, "User not found\n");
    return 0;
}

// محاكاة دالة البحث get_user_info
void get_user_info_sim(const char* username) {
    char output[MAX_OUTPUT_SIZE];
    callback_sim(username, output, sizeof(output));
    printf("%s", output);
}

// دالة الاختبار
void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"ahmad", "username = ahmad\n\n", "Test Case 1: User exists"},
        {"nonexistent", "User not found\n", "Test Case 2: User does not exist"},
        {"john", "username = john\n\n", "Test Case 3: User exists"},
        {"alice", "username = alice\n\n", "Test Case 4: User exists"},
        {"bob", "User not found\n", "Test Case 5: User does not exist"},
    };

    char actual_output[MAX_OUTPUT_SIZE];
    int pass = 1;

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // التحقق من المخرجات
        callback_sim(tests[i].input, actual_output, sizeof(actual_output));

        printf("Running %s... ", tests[i].description);
        if (strcmp(actual_output, tests[i].expected_output) == 0) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected: %sGot: %s", tests[i].expected_output, actual_output);
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
