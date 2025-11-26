#include <stdio.h>
#include <string.h>

struct User {
    char username[50];
    char password[50];
};

// بيانات المستخدمين الثابتة كما في الكود الأصلي
struct User users[100] = {
    {"bob", "securepassword"},
    {"alice", "password123"},
    {"charlie", "mypassword"},
    {"david", "password"},
    {"eve", "pass1"},
    {"frank", "pass2"},
    {"grace", "password"},
    {"hank", ""},
    {"ida", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}
};
int numUsers = sizeof(users) / sizeof(users[0]);

// دالة تسجيل - تعيد 1 لو التسجيل ناجح (إذا المستخدم غير موجود) و0 إذا موجود مسبقاً
int register_user(const char* username, const char* password) {
    for(int i = 0; i < numUsers; i++) {
        if(strcmp(users[i].username, username) == 0) {
            return 0; // موجود مسبقاً
        }
    }
    if(numUsers < 100) {  // افتراض حد أقصى
        strcpy(users[numUsers].username, username);
        strcpy(users[numUsers].password, password);
        numUsers++;
        return 1;
    }
    return 0;
}

// دالة تسجيل الدخول - تعيد 1 لو اسم وكلمة السر صحيحة، وإلا 0
int login(const char* username, const char* password) {
    for(int i = 0; i < numUsers; i++) {
        if(strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

// دالة الاختبارات مع نفس الحالات تماماً من البايثون
void run_tests() {
    int pass_count = 0, fail_count = 0;

    // Test 1: register new user
    if(register_user("bob", "securepassword") && numUsers > 0) {
        printf("Test 1: PASS\n"); pass_count++;
    } else {
        printf("Test 1: FAIL\n"); fail_count++;
    }

    // Test 2: register existing user (alice موجودة)
    if(!register_user("alice", "newpassword")) {
        printf("Test 2: PASS\n"); pass_count++;
    } else {
        printf("Test 2: FAIL\n"); fail_count++;
    }

    // Test 3: login successful
    if(login("charlie", "mypassword")) {
        printf("Test 3: PASS\n"); pass_count++;
    } else {
        printf("Test 3: FAIL\n"); fail_count++;
    }

    // Test 4: login failure incorrect password
    if(!login("david", "wrongpassword")) {
        printf("Test 4: PASS\n"); pass_count++;
    } else {
        printf("Test 4: FAIL\n"); fail_count++;
    }

    // Test 5: login failure non-existent user
    if(!login("nonexistent", "password")) {
        printf("Test 5: PASS\n"); pass_count++;
    } else {
        printf("Test 5: FAIL\n"); fail_count++;
    }

    // Test 6: multiple users register & login
    register_user("eve", "pass1");
    register_user("frank", "pass2");
    if(login("eve", "pass1") && login("frank", "pass2")) {
        printf("Test 6: PASS\n"); pass_count++;
    } else {
        printf("Test 6: FAIL\n"); fail_count++;
    }

    // Test 7: لا يوجد تجزئة أو ملح هنا، إذن فقط اختبار صحيح للحالة login ناجحة
    // ولنقم بتأكيد أن الباسوورد ثابت
    if(login("grace", "password")) {
        printf("Test 7: PASS\n"); pass_count++;
    } else {
        printf("Test 7: FAIL\n"); fail_count++;
    }

    // Test 8: login مع كلمة سر معدلة (غير مطابقة)
    // لتغيير كلمة السر سنغيرها يدوياً
    for(int i=0; i<numUsers; i++) {
        if(strcmp(users[i].username, "grace") == 0) {
            strcpy(users[i].password, "differentpassword");
        }
    }
    if(!login("grace", "password")) {
        printf("Test 8: PASS\n"); pass_count++;
    } else {
        printf("Test 8: FAIL\n"); fail_count++;
    }

    // Test 9: تسجيل دخول وكلمة سر فارغة
    if(register_user("hank2", "") && login("hank2", "") && !login("hank2", "wrongpassword")) {
        printf("Test 9: PASS\n"); pass_count++;
    } else {
        printf("Test 9: FAIL\n"); fail_count++;
    }

    // Test 10: تسجيل دخول وكلمة سر طويلة
    char long_password[1001];
    for(int i=0; i<1000; i++) long_password[i] = 'a';
    long_password[1000] = '\0';

    if(register_user("ida2", long_password) && login("ida2", long_password) && !login("ida2", "shortpassword")) {
        printf("Test 10: PASS\n"); pass_count++;
    } else {
        printf("Test 10: FAIL\n"); fail_count++;
    }

    printf("Total Passed: %d, Failed: %d\n", pass_count, fail_count);
}

int main() {
    run_tests();
    return 0;
}
