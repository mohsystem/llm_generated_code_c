#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

char* createTempFileWithUnicode() {
    char *tempFilePath = NULL;
    char tempFileName[] = "/tmp/tempXXXXXX";
    int fd;

    // Generate a random string
    char randomString[] = "TestUnicodeString"; // Placeholder

    fd = mkstemp(tempFileName);
    if (fd == -1) {
        perror("Error creating temporary file");
        return NULL;
    }

    tempFilePath = strdup(tempFileName);
    if (tempFilePath == NULL) {
        perror("Error duplicating string");
        close(fd);
        unlink(tempFileName);
        return NULL;
    }

    size_t bytesWritten = write(fd, randomString, strlen(randomString));
    if (bytesWritten != strlen(randomString)) {
        perror("Error writing to temporary file");
        close(fd);
        unlink(tempFilePath);
        free(tempFilePath);
        return NULL;
    }

    close(fd);
    return tempFilePath;
}

int check_file_exists_and_not_empty(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

int main() {
    char* tempFilePath = createTempFileWithUnicode();
    if (tempFilePath == NULL) {
        printf("Test case 1: fail (temp file creation failed)\n");
        return 1;
    }
    printf("Temporary file created at: %s\n", tempFilePath);

    if (check_file_exists_and_not_empty(tempFilePath)) {
        printf("Test case 1: pass (file exists and not empty)\n");
    } else {
        printf("Test case 1: fail (file missing or empty)\n");
        free(tempFilePath);
        return 1;
    }

    free(tempFilePath);
    return 0;
}
