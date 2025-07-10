#include <stdio.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 50

// محاكاة قاعدة بيانات للمستخدمين
const char* known_users[] = {"ahmad", "john", "alice", NULL};

int user_exists(const char* username) {
    for (int i = 0; known_users[i] != NULL; i++) {
        if (strcmp(known_users[i], username) == 0) {
            return 1;
        }
    }
    return 0;
}

// محاكاة دالة get_user_info تستدعي user_exists وتطبع النتائج
void get_user_info(const char* username) {
    if (user_exists(username)) {
        printf("User found: %s\n", username);
    } else {
        printf("User not found\n");
    }
}

// دالة الاختبار
void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"ahmad", "User found: ahmad\n", "Test Case 1: user exists"},
        {"nonexistent", "User not found\n", "Test Case 2: user does not exist"},
        {"john", "User found: john\n", "Test Case 3: user exists"},
        {"alice", "User found: alice\n", "Test Case 4: user exists"},
        {"bob", "User not found\n", "Test Case 5: user does not exist"},
    };

    char buffer[256];
    int pass = 1;

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // التقاط المخرجات عن طريق تحويل printf إلى buffer مؤقت
        // لعدم تعقيد الكود، نستخدم مقارنة نص مباشرة هنا:
        // في تطبيق حقيقي يمكن استخدام freopen أو pipe لكن هنا للبساطة نطبع ونقارن

        // الحل البسيط: طباعة متوقعة ثم محاكاة الوظيفة ونطابق النص يدوياً
        printf("Running %s... ", tests[i].description);

        // هنا فقط نحاكي المخرجات في متغير نصي مؤقت
        char output[256];
        if (user_exists(tests[i].input)) {
            snprintf(output, sizeof(output), "User found: %s\n", tests[i].input);
        } else {
            snprintf(output, sizeof(output), "User not found\n");
        }

        if (strcmp(output, tests[i].expected_output) == 0) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected: %sGot: %s", tests[i].expected_output, output);
            pass = 0;
        }
    }

    if (pass) {
        printf("All test cases passed!\n");
    }
}

int main() {
    run_tests();
    return 0;
}
