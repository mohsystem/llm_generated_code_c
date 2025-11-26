#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int build_command(char* command, size_t size, const char* username, const char* newPassword) {
    if (strlen(username) == 0 || strlen(newPassword) == 0) return 0;
    snprintf(command, size, "echo '%s:%s' | sudo chpasswd", username, newPassword);
    return 1;
}

void test_case(const char* test_name, int condition, int expected) {
    if (condition == expected) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected %d, got %d)\n", test_name, expected, condition);
    }
}

int main() {
    char command[256];

    test_case("test_valid_command_build", build_command(command, sizeof(command), "user", "pass123"), 1);
    test_case("test_empty_username", build_command(command, sizeof(command), "", "pass123"), 0);
    test_case("test_empty_password", build_command(command, sizeof(command), "user", ""), 0);

    // تحقق من صياغة الأمر
    build_command(command, sizeof(command), "testuser", "testpass");
    test_case("test_command_format", strcmp(command, "echo 'testuser:testpass' | sudo chpasswd") == 0, 1);

    return 0;
}
