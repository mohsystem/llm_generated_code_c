#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_PERMISSIONS 10

typedef struct {
    char username[50];
    char permissions[MAX_PERMISSIONS][50];
    int permission_count;
} User;

void add_permission(User* user, const char* permission) {
    // منع تكرار الصلاحيات
    for (int i = 0; i < user->permission_count; i++) {
        if (strcmp(user->permissions[i], permission) == 0) {
            return; // صلاحية موجودة مسبقاً، لا تضفها مرة أخرى
        }
    }
    if (user->permission_count < MAX_PERMISSIONS) {
        strcpy(user->permissions[user->permission_count], permission);
        user->permission_count++;
    }
}

int has_permission(const User* user, const char* permission) {
    for (int i = 0; i < user->permission_count; ++i) {
        if (strcmp(user->permissions[i], permission) == 0) {
            return 1;
        }
    }
    return 0;
}

void run_tests() {
    User admin = {"admin", {}, 0};

    add_permission(&admin, "read");
    add_permission(&admin, "write");

    // اختبارات صارمة
    assert(has_permission(&admin, "read") == 1);
    assert(has_permission(&admin, "write") == 1);
    assert(has_permission(&admin, "delete") == 0);

    // اختبار عدم إضافة صلاحيات مكررة
    add_permission(&admin, "read");  // يجب أن لا تزيد العد

    assert(admin.permission_count == 2);

    // اختبار تجاوز الحد الأقصى للصلاحيات
    for (int i = admin.permission_count; i < MAX_PERMISSIONS + 5; i++) {
        char perm[20];
        sprintf(perm, "perm%d", i);
        add_permission(&admin, perm);  // يجب أن يتوقف عند MAX_PERMISSIONS
    }
    assert(admin.permission_count == MAX_PERMISSIONS);

    printf("✓ All tests passed for minimal permission system.\n");
}

int main() {
    run_tests();
    return 0;
}
