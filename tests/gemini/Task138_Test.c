#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_PERMISSIONS 50
#define MAX_USERNAME_LENGTH 50
#define MAX_PERMISSION_LENGTH 50

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char permissions[MAX_PERMISSIONS][MAX_PERMISSION_LENGTH];
    int permission_count;
} User;

User users[MAX_USERS];
int user_count = 0;

int find_user(const char *username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

void add_user(const char *username) {
    if (user_count < MAX_USERS) {
        if(find_user(username) != -1) return; // لا تضيف مستخدم مكرر
        strcpy(users[user_count].username, username);
        users[user_count].permission_count = 0;
        user_count++;
    }
}

void add_permission(const char *username, const char *permission) {
    int user_index = find_user(username);
    if (user_index != -1) {
        // تحقق من عدم وجود صلاحية مكررة
        for (int i = 0; i < users[user_index].permission_count; i++) {
            if (strcmp(users[user_index].permissions[i], permission) == 0) {
                return;
            }
        }
        if (users[user_index].permission_count < MAX_PERMISSIONS) {
            strcpy(users[user_index].permissions[users[user_index].permission_count], permission);
            users[user_index].permission_count++;
        }
    }
}

void remove_permission(const char *username, const char *permission) {
    int user_index = find_user(username);
    if (user_index != -1) {
        for (int i = 0; i < users[user_index].permission_count; i++) {
            if (strcmp(users[user_index].permissions[i], permission) == 0) {
                // تحريك الصلاحيات بعد الحذف
                for (int j = i; j < users[user_index].permission_count - 1; j++) {
                    strcpy(users[user_index].permissions[j], users[user_index].permissions[j + 1]);
                }
                users[user_index].permission_count--;
                return;
            }
        }
    }
}

int has_permission(const char *username, const char *permission) {
    int user_index = find_user(username);
    if (user_index != -1) {
        for (int i = 0; i < users[user_index].permission_count; i++) {
            if (strcmp(users[user_index].permissions[i], permission) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

void run_tests() {
    // إضافة المستخدمين والصلاحيات مثل البايثون
    add_user("alice");
    add_permission("alice", "read");
    add_permission("alice", "write");
    add_permission("alice", "delete");
    add_permission("alice", "manage_users");

    add_user("bob");
    add_permission("bob", "read");
    add_permission("bob", "write");

    add_user("charlie");
    add_permission("charlie", "read");

    // الاختبارات (تطبع 1 للصح و0 للخطأ)
    printf("%d\n", has_permission("alice", "manage_users"));  // 1
    printf("%d\n", has_permission("bob", "write"));          // 1
    printf("%d\n", has_permission("charlie", "delete"));     // 0

    // تغيير صلاحيات bob ليكون admin (نمسح صلاحياته القديمة ونضيف صلاحيات admin)
    // أولاً نحذف bob (نمسح صلاحياته)
    int bob_index = find_user("bob");
    if (bob_index != -1) {
        users[bob_index].permission_count = 0;
    }
    // نضيف صلاحيات admin
    add_permission("bob", "read");
    add_permission("bob", "write");
    add_permission("bob", "delete");
    add_permission("bob", "manage_users");

    printf("%d\n", has_permission("bob", "manage_users"));   // 1

    // إزالة charlie
    int charlie_index = find_user("charlie");
    if (charlie_index != -1) {
        // إزالة المستخدم عن طريق تحريك بقية المستخدمين
        for (int i = charlie_index; i < user_count - 1; i++) {
            users[i] = users[i + 1];
        }
        user_count--;
    }

    printf("%d\n", has_permission("charlie", "read"));       // 0
}

int main() {
    run_tests();
    return 0;
}
