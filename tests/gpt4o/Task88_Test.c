#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>

#define STRING_LENGTH 10

int check_file_exists_and_not_empty(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

int main() {
    // Generate a random string
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char randomString[STRING_LENGTH + 1];
    srand((unsigned) time(NULL));
    for (int i = 0; i < STRING_LENGTH; ++i) {
        randomString[i] = characters[rand() % (sizeof(characters) - 1)];
    }
    randomString[STRING_LENGTH] = '\0';

    // Convert string into Unicode
    setlocale(LC_ALL, "");
    size_t len = mbstowcs(NULL, randomString, 0);
    wchar_t *unicodeString = (wchar_t *) malloc((len + 1) * sizeof(wchar_t));
    mbstowcs(unicodeString, randomString, len + 1);

    // Create a temporary file and write the Unicode string to it
    char tempFilePath[] = "/tmp/tempfileXXXXXX";
    int tempFileDesc = mkstemp(tempFilePath);
    FILE *tempFile = fdopen(tempFileDesc, "w");
    fwprintf(tempFile, L"%ls", unicodeString);
    fclose(tempFile);

    // Output the path of the temporary file
    printf("%s\n", tempFilePath);

    // Check file exists and is not empty
    if (check_file_exists_and_not_empty(tempFilePath)) {
        printf("Test case 1: pass (file exists and not empty)\n");
    } else {
        printf("Test case 1: fail (file missing or empty)\n");
    }

    // Free allocated memory
    free(unicodeString);

    return 0;
}
