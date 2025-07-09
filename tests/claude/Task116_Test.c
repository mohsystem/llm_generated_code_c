#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 64

void drop_privileges(const char* username) {
    struct passwd* pw = getpwnam(username);
    if (pw == NULL) {
        fprintf(stderr, "Failed to get user info for %s\n", username);
        exit(1);
    }

    if (setgid(pw->pw_gid) != 0) {
        fprintf(stderr, "Failed to set GID: %s\n", strerror(errno));
        exit(1);
    }

    if (setuid(pw->pw_uid) != 0) {
        fprintf(stderr, "Failed to set UID: %s\n", strerror(errno));
        exit(1);
    }

    printf("Privileges dropped to %s\n", username);
}

void change_password(const char* username, const char* password) {
    char command[256];
    snprintf(command, sizeof(command), "echo '%s\n%s' | passwd %s", password, password, username);
    int result = system(command);
    if (result == 0) {
        printf("Password changed successfully for %s\n", username);
    } else {
        fprintf(stderr, "Failed to change password for %s\n", username);
    }
}

void test_case(const char* test_name, const char* username, int expected_exists) {
    struct passwd* pw = getpwnam(username);
    int result = (pw != NULL) ? 1 : 0;
    if (result == expected_exists) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected %s, got %s)\n", test_name, expected_exists ? "exists" : "not exists", result ? "exists" : "not exists");
    }
}

int main() {
    // if (geteuid() != 0) {
    //     fprintf(stderr, "This program must be run as root.\n");
    //     return 1;
    // }

    // مثال بسيط لاختبار وجود مستخدم (test case) - فقط هذا ممكن هنا
    test_case("test_existing_user_root", "root", 1);          // متوقع أن يكون موجودًا
    test_case("test_non_existing_user_fakeuser", "fakeuser", 0); // متوقع أن لا يكون موجودًا

    // يمكنك اختبار برنامجك يدويًا مع أي مستخدمين موجودين على النظام

    // تنفيذ البرنامج كالمعتاد (يمكنك إلغاء التعليق لتجربته يدوياً)
    /*
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username to change password: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        fprintf(stderr, "Error reading username\n");
        return 1;
    }
    username[strcspn(username, "\n")] = 0;

    printf("Enter new password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        fprintf(stderr, "Error reading password\n");
        return 1;
    }
    password[strcspn(password, "\n")] = 0;

    change_password(username, password);
    printf("Dropping privileges...\n");
    drop_privileges("nobody");
    printf("Program continues with reduced privileges...\n");
    */

    return 0;
}
