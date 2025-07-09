#include <stdio.h>
#include <pwd.h>
#include <string.h>

void test_case(const char* test_name, const char* username, int expected_exists) {
    struct passwd *pw = getpwnam(username);
    int exists = (pw != NULL) ? 1 : 0;
    if (exists == expected_exists) {
        printf("%s: pass\n", test_name);
    } else {
        printf("%s: fail (expected %s, got %s)\n", test_name,
               expected_exists ? "exists" : "not exists",
               exists ? "exists" : "not exists");
    }
}

int main() {
    test_case("test_existing_user_root", "root", 1);
    test_case("test_non_existing_user_fakeuser", "fakeuser", 0);

    return 0;
}
