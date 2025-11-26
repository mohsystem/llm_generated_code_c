#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    FILE *script = fopen("script.sh", "w");
    if (script == NULL) {
        perror("Error opening file");
        printf("Test result: FAIL (file open)\n");
        return 1;
    }

    fprintf(script, "#!/bin/bash\n");
    fprintf(script, "echo \"Hello from script.sh!\"\n");
    fclose(script);

    if (system("chmod +x script.sh") != 0) {
        perror("Error making script executable");
        printf("Test result: FAIL (chmod)\n");
        return 1;
    }

    // تحقق من محتوى الملف
    script = fopen("script.sh", "r");
    if (!script) {
        printf("Test result: FAIL (file open for verification)\n");
        return 1;
    }

    char buffer[256];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, script);
    buffer[len] = '\0';
    fclose(script);

    const char *expected_content = "#!/bin/bash\n"
                                   "echo \"Hello from script.sh!\"\n";

    if (strcmp(buffer, expected_content) != 0) {
        printf("Test result: FAIL (content mismatch)\n");
        return 1;
    }

    // تحقق من صلاحيات الملف
    struct stat st;
    if (stat("script.sh", &st) != 0) {
        perror("stat");
        printf("Test result: FAIL (stat)\n");
        return 1;
    }

    mode_t expected_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 755
    if ((st.st_mode & 0777) != expected_mode) {
        printf("Test result: FAIL (permissions mismatch)\n");
        return 1;
    }

    printf("Test result: PASS\n");
    return 0;
}
