#include <stdio.h>
#include <string.h>

int is_zip(const char* path) {
    size_t len = strlen(path);
    return (len > 4 && strcmp(path + len - 4, ".zip") == 0);
}

int is_tar(const char* path) {
    size_t len = strlen(path);
    return (len > 4 && strcmp(path + len - 4, ".tar") == 0);
}

void test_case(const char* test_name, const char* archive_path, const char* expected) {
    const char* result;
    if (is_zip(archive_path)) {
        result = "ZIP";
    } else if (is_tar(archive_path)) {
        result = "TAR";
    } else {
        result = "Unsupported";
    }

    if ((strcmp(expected, "ZIP") == 0 && strcmp(result, "ZIP") == 0) ||
        (strcmp(expected, "TAR") == 0 && strcmp(result, "TAR") == 0) ||
        (strcmp(expected, "Unsupported") == 0 && strcmp(result, "Unsupported") == 0)) {
        printf("%s: pass\n", test_name);
        } else {
            printf("%s: fail (expected %s, got %s)\n", test_name, expected, result);
        }
}

int main() {
    test_case("test_zip_file", "archive.zip", "ZIP");
    test_case("test_tar_file", "backup.tar", "TAR");
    test_case("test_unsupported_file", "document.txt", "Unsupported");
    test_case("test_zip_longer_name", "/home/user/files/data.zip", "ZIP");
    test_case("test_tar_longer_name", "/var/backups/archive.tar", "TAR");
    test_case("test_unsupported_no_extension", "/tmp/file", "Unsupported");

    return 0;
}
