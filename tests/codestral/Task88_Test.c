#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

char* generateRandomString(int length) {
    char* randomString = malloc(sizeof(char) * (length + 1));
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        randomString[i] = characters[rand() % strlen(characters)];
    }
    randomString[length] = '\0';
    return randomString;
}

char* stringToUnicode(const char* input) {
    int length = strlen(input);
    char* unicodeString = malloc(sizeof(char) * (length * 6 + 1));
    int index = 0;
    for (int i = 0; i < length; i++) {
        sprintf(unicodeString + index, "\\u%04x", input[i]);
        index += 6;
    }
    unicodeString[index] = '\0';
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

    char buffer[1000];
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
    FILE* tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        printf("Test case 1: fail (could not create file)\n");
        return 1;
    }

    char* randomString = generateRandomString(10);
    char* unicodeString = stringToUnicode(randomString);

    fprintf(tempFile, "%s", unicodeString);
    fclose(tempFile);

    printf("temp.txt\n");

    if (check_file_exists_and_not_empty("temp.txt")) {
        printf("Test case 1: pass (file exists and not empty)\n");
    } else {
        printf("Test case 1: fail (file missing or empty)\n");
        free(randomString);
        free(unicodeString);
        return 1;
    }

    if (validate_unicode_content("temp.txt")) {
        printf("Test case 1: pass (unicode content validated)\n");
    } else {
        printf("Test case 1: fail (unicode content invalid)\n");
    }

    free(randomString);
    free(unicodeString);

    return 0;
}
