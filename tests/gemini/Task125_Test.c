#include <stdio.h>
#include <string.h>

// الكود الأصلي:
struct Resource {
    char name[50];
};

struct Role {
    char name[50];
    struct Resource resources[3];
    int resourceCount;
};

struct Role roles[3] = {
    {"admin", { {"resource1"}, {"resource2"}, {"resource3"} }, 3},
    {"editor", { {"resource2"}, {"resource3"} }, 2},
    {"viewer", { {"resource3"} }, 1}
};
// === Test framework ===
#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

int role_has_resource(struct Role* role, const char* resource) {
    for (int i = 0; i < role->resourceCount; i++) {
        if (strcmp(role->resources[i].name, resource) == 0) {
            return 1;
        }
    }
    return 0;
}

// --- التست كيسز من البايثون (تأكيد صلاحيات الأدوار) ---

void test_role_admin_resources() {
    ASSERT(role_has_resource(&roles[0], "resource1") &&
           role_has_resource(&roles[0], "resource2") &&
           role_has_resource(&roles[0], "resource3"), "test_role_admin_resources");
}

void test_role_editor_resources() {
    ASSERT(!role_has_resource(&roles[1], "resource1") &&
           role_has_resource(&roles[1], "resource2") &&
           role_has_resource(&roles[1], "resource3"), "test_role_editor_resources");
}

void test_role_viewer_resources() {
    ASSERT(!role_has_resource(&roles[2], "resource1") &&
           !role_has_resource(&roles[2], "resource2") &&
           role_has_resource(&roles[2], "resource3"), "test_role_viewer_resources");
}

void test_invalid_role() {
    int found = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp("invalid_role", roles[i].name) == 0) {
            found = 1;
        }
    }
    ASSERT(!found, "test_invalid_role");
}

// اختبارات إضافية منطقية لقوة التحقق

void test_empty_role_string() {
    int found = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp("", roles[i].name) == 0) {
            found = 1;
        }
    }
    ASSERT(!found, "test_empty_role_string");
}

void test_role_case_sensitive() {
    int found = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp("Admin", roles[i].name) == 0) { // "Admin" بدل "admin"
            found = 1;
        }
    }
    ASSERT(!found, "test_role_case_sensitive");
}

void test_role_trailing_spaces() {
    int found = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp("admin ", roles[i].name) == 0) { // "admin " مع مسافة في النهاية
            found = 1;
        }
    }
    ASSERT(!found, "test_role_trailing_spaces");
}

// اختبار التفاعل مع المستخدم (الإدخال/الإخراج) غير ممكن بدون تعديل الكود
void test_input_output_interaction() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_input_output_interaction");
}

int main() {
    test_role_admin_resources();
    test_role_editor_resources();
    test_role_viewer_resources();
    test_invalid_role();

    test_empty_role_string();
    test_role_case_sensitive();
    test_role_trailing_spaces();

    test_input_output_interaction();

    return 0;
}
