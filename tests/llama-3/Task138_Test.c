#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 100
#define MAX_ROLE_NAME 20
#define MAX_PERMISSION_NAME 20
#define MAX_PERMISSIONS_PER_ROLE 10
#define MAX_USERNAME 50

typedef struct {
    char role_name[MAX_ROLE_NAME];
    char permissions[MAX_PERMISSIONS_PER_ROLE][MAX_PERMISSION_NAME];
    int permission_count;
} Role;

typedef struct {
    char username[MAX_USERNAME];
    char role_name[MAX_ROLE_NAME];
} User;

typedef struct {
    User users[MAX_USERS];
    int user_count;

    Role roles[3];
    int role_count;
} PermissionManager;

// تهيئة الأدوار وصلاحياتها
void init_roles(PermissionManager *pm) {
    pm->role_count = 3;

    strcpy(pm->roles[0].role_name, "admin");
    char *admin_perms[] = {"read", "write", "delete", "manage_users"};
    pm->roles[0].permission_count = 4;
    for (int i=0; i<4; i++) {
        strcpy(pm->roles[0].permissions[i], admin_perms[i]);
    }

    strcpy(pm->roles[1].role_name, "editor");
    char *editor_perms[] = {"read", "write"};
    pm->roles[1].permission_count = 2;
    for (int i=0; i<2; i++) {
        strcpy(pm->roles[1].permissions[i], editor_perms[i]);
    }

    strcpy(pm->roles[2].role_name, "viewer");
    char *viewer_perms[] = {"read"};
    pm->roles[2].permission_count = 1;
    for (int i=0; i<1; i++) {
        strcpy(pm->roles[2].permissions[i], viewer_perms[i]);
    }

    pm->user_count = 0;
}

// وظيفة مساعدة للبحث عن الدور حسب الاسم
Role* find_role(PermissionManager *pm, const char *role_name) {
    for (int i=0; i<pm->role_count; i++) {
        if (strcmp(pm->roles[i].role_name, role_name) == 0) {
            return &pm->roles[i];
        }
    }
    return NULL;
}

// إضافة مستخدم مع دور معين
bool add_user(PermissionManager *pm, const char *username, const char *role_name) {
    if (pm->user_count >= MAX_USERS) return false;
    if (find_role(pm, role_name) == NULL) return false; // دور غير صالح

    // تحقق إذا المستخدم موجود مسبقًا، نغير دوره
    for (int i=0; i<pm->user_count; i++) {
        if (strcmp(pm->users[i].username, username) == 0) {
            strcpy(pm->users[i].role_name, role_name);
            return true;
        }
    }

    strcpy(pm->users[pm->user_count].username, username);
    strcpy(pm->users[pm->user_count].role_name, role_name);
    pm->user_count++;
    return true;
}

// إزالة مستخدم
bool remove_user(PermissionManager *pm, const char *username) {
    for (int i=0; i<pm->user_count; i++) {
        if (strcmp(pm->users[i].username, username) == 0) {
            // تحريك كل المستخدمين إلى الخلف لتفريغ مكان
            for (int j=i; j<pm->user_count - 1; j++) {
                pm->users[j] = pm->users[j+1];
            }
            pm->user_count--;
            return true;
        }
    }
    return false;
}

// تغيير دور مستخدم موجود
bool change_user_role(PermissionManager *pm, const char *username, const char *new_role) {
    Role *role = find_role(pm, new_role);
    if (!role) return false; // دور غير صالح

    for (int i=0; i<pm->user_count; i++) {
        if (strcmp(pm->users[i].username, username) == 0) {
            strcpy(pm->users[i].role_name, new_role);
            return true;
        }
    }
    return false; // المستخدم غير موجود
}

// التحقق من صلاحية المستخدم
bool check_permission(PermissionManager *pm, const char *username, const char *permission) {
    // العثور على المستخدم
    for (int i=0; i<pm->user_count; i++) {
        if (strcmp(pm->users[i].username, username) == 0) {
            Role *role = find_role(pm, pm->users[i].role_name);
            if (!role) return false;

            for (int j=0; j<role->permission_count; j++) {
                if (strcmp(role->permissions[j], permission) == 0) {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

int main() {
    PermissionManager pm;
    init_roles(&pm);

    add_user(&pm, "alice", "admin");
    add_user(&pm, "bob", "editor");
    add_user(&pm, "charlie", "viewer");

    printf("%d\n", check_permission(&pm, "alice", "manage_users")); // 1 (true)
    printf("%d\n", check_permission(&pm, "bob", "write")); // 1 (true)
    printf("%d\n", check_permission(&pm, "charlie", "delete")); // 0 (false)

    change_user_role(&pm, "bob", "admin");
    printf("%d\n", check_permission(&pm, "bob", "manage_users")); // 1 (true)

    remove_user(&pm, "charlie");
    printf("%d\n", check_permission(&pm, "charlie", "read")); // 0 (false)

    return 0;
}
