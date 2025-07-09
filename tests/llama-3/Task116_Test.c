#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
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
    // مثال اختبار صلاحيات root
    test_case("test_run_as_root", is_root(), 1);

    // مثال اختبار وجود المستخدم nobody
    test_case("test_existing_user_nobody", user_exists("nobody"), 1);

    // مثال اختبار وجود مستخدم وهمي
    test_case("test_non_existing_user_fakeuser", user_exists("fakeuser"), 0);

    // اختبار بناء الأمر (يمكن توسيع لفحص دقيق)
    char username[] = "testuser";
    char password[] = "testpass";
    char command[512];
    snprintf(command, sizeof(command), "echo '%s' | sudo -S passwd %s", password, username);
    const char* expected_command = "echo 'testpass' | sudo -S passwd testuser";
    test_case("test_command_format", strcmp(command, expected_command) == 0, 1);

    return 0;
}
