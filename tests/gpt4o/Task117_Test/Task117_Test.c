#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Helper to check if directory exists
int dir_exists(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0 && (st.st_mode & S_IFDIR)) return 1;
    return 0;
}

// Run command, redirect stderr to temp file, returns 1 if "Unsupported file type" found on stderr
int run_and_check(const char *cmd, const char *extract_dir, int expect_supported) {
    system("rm -rf temp_extract_dir");
    char full_cmd[512];
    snprintf(full_cmd, sizeof(full_cmd), "%s > /dev/null 2> err.log", cmd);
    int ret = system(full_cmd);

    int unsupported_found = 0;
    FILE *f = fopen("err.log", "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "Unsupported file type") != NULL) {
                unsupported_found = 1;
                break;
            }
        }
        fclose(f);
    }
    if (expect_supported) {
        // We expect no error and extraction dir exists
        if (unsupported_found) return 0; // fail
        if (!dir_exists(extract_dir)) return 0; // fail
        return 1; // pass
    } else {
        // We expect unsupported error
        return unsupported_found ? 1 : 0;
    }
}

int main() {
    // Test 1: Valid ZIP archive extraction
    int pass1 = run_and_check("./program test.zip temp_extract_dir", "temp_extract_dir", 1);

    // Test 2: Valid TAR archive extraction
    int pass2 = run_and_check("./program test.tar temp_extract_dir", "temp_extract_dir", 1);

    // Test 3: Unsupported archive extraction
    int pass3 = run_and_check("./program test.txt temp_extract_dir", "temp_extract_dir", 0);

    printf("Test 1 (ZIP): %s\n", pass1 ? "PASS" : "FAIL");
    printf("Test 2 (TAR): %s\n", pass2 ? "PASS" : "FAIL");
    printf("Test 3 (Unsupported): %s\n", pass3 ? "PASS" : "FAIL");

    return 0;
}
