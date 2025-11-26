#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* getRandomString() {
    char* SALTCHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char* salt = (char*)malloc(11 * sizeof(char));
    srand(time(0));
    for (int i = 0; i < 10; i++) {
        salt[i] = SALTCHARS[rand() % strlen(SALTCHARS)];
    }
    salt[10] = '\0';
    return salt;
}

char* toUnicode(const char* str) {
    char* unicodeString = (char*)malloc((strlen(str) * 6 + 1) * sizeof(char));
    char* p = unicodeString;
    for (; *str; str++) {
        sprintf(p, "\\u%04x", *str);
        p += 6;
    }
    *p = '\0';
    return unicodeString;
}

int check_file_exists_and_not_empty(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

int validate_unicode_content(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;

    char buffer[2000];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[read_size] = '\0';
    fclose(fp);

    if (strlen(buffer) < 60) return 0;

    int count = 0;
    for (size_t i = 0; i < strlen(buffer) - 5; i++) {
        if (buffer[i] == '\\' && buffer[i+1] == 'u') {
            int valid = 1;
            for (int j = 2; j <= 5; j++) {
                char c = buffer[i + j];
                if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                count++;
                i += 5;
            }
        }
    }
    return (count == 10);
}

int main() {
    char* randomString = getRandomString();
    char* unicodeString = toUnicode(randomString);

    char tempFileName[256];
    sprintf(tempFileName, "temp_%s.txt", getRandomString());

    FILE* tempFile = fopen(tempFileName, "wb");
    if (!tempFile) {
        printf("Test case 1: fail (cannot create file)\n");
        free(randomString);
        free(unicodeString);
        return 1;
    }
    fwrite(unicodeString, strlen(unicodeString), 1, tempFile);
    fclose(tempFile);

    printf("%s\n", tempFileName);

    if (check_file_exists_and_not_empty(tempFileName)) {
        printf("Test case 1: pass (file exists and not empty)\n");
    } else {
        printf("Test case 1: fail (file missing or empty)\n");
        free(randomString);
        free(unicodeString);
        return 1;
    }

    if (validate_unicode_content(tempFileName)) {
        printf("Test case 1: pass (unicode content validated)\n");
    } else {
        printf("Test case 1: fail (unicode content invalid)\n");
    }

    free(randomString);
    free(unicodeString);
    return 0;
}
