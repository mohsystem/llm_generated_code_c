#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_PERMISSIONS 10

typedef struct {
    char name[50];
    char permissions[MAX_PERMISSIONS][50];
    int num_permissions;
} User;

bool has_permission(User* user, const char* permission) {
    for (int i = 0; i < user->num_permissions; i++) {
        if (strcmp(user->permissions[i], permission) == 0) {
            return true;
        }
    }
    return false;
}

void run_tests() {
    User user = {"John", {"read", "write"}, 2};

    // ✅ اختبارات صارمة باستخدام assert
    assert(has_permission(&user, "read") == true);
    assert(has_permission(&user, "write") == true);
    assert(has_permission(&user, "execute") == false);
    assert(has_permission(&user, "delete") == false);

    puts("✓ All tests passed for has_permission function.");
}

int main() {
    run_tests();
    return 0;
}
