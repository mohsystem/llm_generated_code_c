#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    FILE *fp = fopen("script.sh", "w");
    if (fp != NULL) {
        fprintf(fp, "#!/bin/bash\necho \"Hello, World!\"");
        fclose(fp);

        int ret = system("chmod +x script.sh");
        if (ret != 0) {
            printf("Test result: FAIL (chmod command failed)\n");
            return 1;
        }

        // تحقق من محتوى الملف
        fp = fopen("script.sh", "r");
        if (!fp) {
            printf("Test result: FAIL (cannot open file for verification)\n");
            return 1;
        }

        char buffer[256];
        size_t len = fread(buffer, 1, sizeof(buffer) - 1, fp);
        buffer[len] = '\0';
        fclose(fp);

        const char *expected_content = "#!/bin/bash\necho \"Hello, World!\"";
        if (strcmp(buffer, expected_content) != 0) {
            printf("Test result: FAIL (content mismatch)\n");
            return 1;
        }

        // تحقق من صلاحيات الملف
        struct stat st;
        if (stat("script.sh", &st) != 0) {
            perror("stat");
            printf("Test result: FAIL (stat failed)\n");
            return 1;
        }

        mode_t expected_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 755
        if ((st.st_mode & 0777) != expected_mode) {
            printf("Test result: FAIL (permissions mismatch)\n");
            return 1;
        }

        printf("Test result: PASS\n");
        return 0;

    } else {
        printf("Unable to open file\n");
        printf("Test result: FAIL (file open)\n");
        return 1;
    }
}
