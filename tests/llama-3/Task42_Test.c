#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char username[20];
    char password[20];
} User;

bool authenticate(User user, char* username, char* password) {
    return strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0;
}

void run_tests() {
    User users[2] = {{"user1", "password1"}, {"user2", "password2"}};

    struct {
        char* username;
        char* password;
        bool expected;
        char* description;
    } tests[] = {
        {"user1", "password1", true, "Test 1: valid user1 correct password"},
        {"user2", "password2", true, "Test 2: valid user2 correct password"},
        {"user1", "wrongpass", false, "Test 3: valid user1 wrong password"},
        {"user2", "wrongpass", false, "Test 4: valid user2 wrong password"},
        {"nonexistent", "password", false, "Test 5: nonexistent user"},
        {"user1", "", false, "Test 6: user1 empty password"},
        {"", "password1", false, "Test 7: empty username"},
        {"", "", false, "Test 8: empty username and password"},
        {"user2", "PASSWORD2", false, "Test 9: valid user2 wrong case password"},
        {"USER1", "password1", false, "Test 10: wrong case username"},
    };

    int pass = 0, fail = 0;
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < n; i++) {
        bool auth_success = false;
        for (int j = 0; j < 2; j++) {
            if (authenticate(users[j], tests[i].username, tests[i].password)) {
                auth_success = true;
                break;
            }
        }

        printf("%s... ", tests[i].description);
        if (auth_success == tests[i].expected) {
            printf("PASS\n");
            pass++;
        } else {
            printf("FAIL\n");
        }
    }

    printf("Total Passed: %d, Failed: %d\n", pass, fail);
}

int main() {
    run_tests();
    return 0;
}
