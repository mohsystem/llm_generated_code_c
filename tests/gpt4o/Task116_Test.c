#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>

int is_root() {
    return geteuid() == 0;
}

int user_exists(const char* username) {
    return getpwnam(username) != NULL;
}

void test_case(const char* test_name, int condition, int expected) {
    if (condition == expected) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected %d, got %d)\n", test_name, expected, condition);
    }
}

int main() {
    // اختبار صلاحيات root
    test_case("test_run_as_root", is_root(), 1);

    // اختبار وجود مستخدم معروف (مثلاً root)
    test_case("test_existing_user_root", user_exists("root"), 1);

    // اختبار وجود مستخدم وهمي (غير موجود)
    test_case("test_non_existing_user_fakeuser", user_exists("fakeuser"), 0);

    // لا يمكن اختبار تغيير كلمة المرور بشكل أوتوماتيكي هنا

    return 0;
}
