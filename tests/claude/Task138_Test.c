#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_USERS 100
#define MAX_ROLES 3
#define MAX_PERMISSIONS 4
#define MAX_USERNAME 50
#define MAX_ROLE 20

struct User {
    char username[MAX_USERNAME];
    char role[MAX_ROLE];
};

struct Role {
    char name[MAX_ROLE];
    char permissions[MAX_PERMISSIONS][MAX_ROLE];
    int num_permissions;
};

struct PermissionManager {
    struct User users[MAX_USERS];
    int num_users;
    struct Role roles[MAX_ROLES];
    int num_roles;
};

void init_permission_manager(struct PermissionManager* pm) {
    pm->num_users = 0;
    pm->num_roles = 3;

    strcpy(pm->roles[0].name, "admin");
    pm->roles[0].num_permissions = 4;
    strcpy(pm->roles[0].permissions[0], "read");
    strcpy(pm->roles[0].permissions[1], "write");
    strcpy(pm->roles[0].permissions[2], "delete");
    strcpy(pm->roles[0].permissions[3], "manage_users");

    strcpy(pm->roles[1].name, "editor");
    pm->roles[1].num_permissions = 2;
    strcpy(pm->roles[1].permissions[0], "read");
    strcpy(pm->roles[1].permissions[1], "write");

    strcpy(pm->roles[2].name, "viewer");
    pm->roles[2].num_permissions = 1;
    strcpy(pm->roles[2].permissions[0], "read");
}

void add_user(struct PermissionManager* pm, const char* username, const char* role) {
    if (pm->num_users >= MAX_USERS) return;

    for (int i = 0; i < pm->num_roles; i++) {
        if (strcmp(pm->roles[i].name, role) == 0) {
            strcpy(pm->users[pm->num_users].username, username);
            strcpy(pm->users[pm->num_users].role, role);
            pm->num_users++;
            return;
        }
    }
}

void remove_user(struct PermissionManager* pm, const char* username) {
    for (int i = 0; i < pm->num_users; i++) {
        if (strcmp(pm->users[i].username, username) == 0) {
            for (int j = i; j < pm->num_users - 1; j++) {
                pm->users[j] = pm->users[j + 1];
            }
            pm->num_users--;
            return;
        }
    }
}

void change_user_role(struct PermissionManager* pm, const char* username, const char* new_role) {
    for (int i = 0; i < pm->num_users; i++) {
        if (strcmp(pm->users[i].username, username) == 0) {
            for (int j = 0; j < pm->num_roles; j++) {
                if (strcmp(pm->roles[j].name, new_role) == 0) {
                    strcpy(pm->users[i].role, new_role);
                    return;
                }
            }
            return;
        }
    }
}

bool check_permission(struct PermissionManager* pm, const char* username, const char* permission) {
    for (int i = 0; i < pm->num_users; i++) {
        if (strcmp(pm->users[i].username, username) == 0) {
            for (int j = 0; j < pm->num_roles; j++) {
                if (strcmp(pm->roles[j].name, pm->users[i].role) == 0) {
                    for (int k = 0; k < pm->roles[j].num_permissions; k++) {
                        if (strcmp(pm->roles[j].permissions[k], permission) == 0) {
                            return true;
                        }
                    }
                    return false;
                }
            }
        }
    }
    return false;
}

/* ✅ اختبارات بدون حفظ/تحميل ملفات */
void run_tests(void) {
    struct PermissionManager pm;
    init_permission_manager(&pm);

    add_user(&pm, "alice", "admin");
    add_user(&pm, "bob", "editor");
    add_user(&pm, "charlie", "viewer");

    assert(check_permission(&pm, "alice", "manage_users") == true);
    assert(check_permission(&pm, "bob", "write") == true);
    assert(check_permission(&pm, "charlie", "delete") == false);

    change_user_role(&pm, "bob", "admin");
    assert(check_permission(&pm, "bob", "manage_users") == true);

    remove_user(&pm, "charlie");
    assert(check_permission(&pm, "charlie", "read") == false);

    // ✅ ديناميكية: نسخ المستخدمين إلى PermissionManager جديد (بدون ملف)
    struct PermissionManager pm_copy;
    init_permission_manager(&pm_copy);
    for (int i = 0; i < pm.num_users; i++) {
        add_user(&pm_copy, pm.users[i].username, pm.users[i].role);
    }

    assert(pm_copy.num_users == 2);
    assert(strcmp(pm_copy.users[0].username, "alice") == 0);
    assert(strcmp(pm_copy.users[0].role, "admin") == 0);
    assert(strcmp(pm_copy.users[1].username, "bob") == 0);
    assert(strcmp(pm_copy.users[1].role, "admin") == 0);

puts(" All tests passed (without using external files).");
}

int main(void) {
    run_tests();
    return 0;
}
