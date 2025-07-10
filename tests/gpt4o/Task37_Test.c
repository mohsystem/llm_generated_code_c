#include <stdio.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 200

// بيانات مستخدمين وهميين (username, info)
struct User {
    const char* username;
    const char* info;
};

struct User known_users[] = {
    {"ahmad", "Info for ahmad"},
    {"john", "Info for john"},
    {"alice", "Info for alice"},
    {NULL, NULL}
};

// محاكاة دالة getUserByUsername
void getUserByUsername_mock(const char* username, char* output, size_t max_output_size) {
    for (int i = 0; known_users[i].username != NULL; i++) {
        if (strcmp(known_users[i].username, username) == 0) {
            snprintf(output, max_output_size, "User: %s - Info: %s\n", known_users[i].username, known_users[i].info);
            return;
        }
    }
    snprintf(output, max_output_size, "User not found\n");
}

// دالة الاختبار
void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"ahmad", "User: ahmad - Info: Info for ahmad\n", "Test Case 1: User exists"},
        {"nonexistent", "User not found\n", "Test Case 2: User does not exist"},
        {"john", "User: john - Info: Info for john\n", "Test Case 3: User exists"},
        {"alice", "User: alice - Info: Info for alice\n", "Test Case 4: User exists"},
        {"bob", "User not found\n", "Test Case 5: User does not exist"},
    };

    char output[MAX_OUTPUT_SIZE];
    int pass = 1;

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        getUserByUsername_mock(tests[i].input, output, sizeof(output));

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
