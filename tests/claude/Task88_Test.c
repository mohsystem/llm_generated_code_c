#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

char* create_temp_file_with_unicode() {
    char temp_filename[] = "tempfileXXXXXX";
    #ifdef _WIN32
    _mktemp(temp_filename);
    #else
    mkstemp(temp_filename);
    #endif

    FILE* temp_file = fopen(temp_filename, "w");
    if (temp_file == NULL) {
        perror("Error creating temporary file");
        return NULL;
    }

    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char random_string[11];
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        random_string[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    random_string[10] = '\0';

    for (int i = 0; i < 10; i++) {
        fprintf(temp_file, "\\\\u%04X", (unsigned int)random_string[i]);
    }

    fclose(temp_file);

    char* result = malloc(strlen(temp_filename) + 1);
    if (result == NULL) {
        perror("Error allocating memory");
        return NULL;
    }
    strcpy(result, temp_filename);
    return result;
}

int check_file_exists_and_not_empty(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

// تحقق أن المحتوى يحتوي على 10 تكرارات للنمط \uXXXX
int validate_unicode_content(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;

    char buffer[1000];
    size_t read_size = fread(buffer, 1, sizeof(buffer)-1, fp);
    buffer[read_size] = '\0';
    fclose(fp);

    // طول المحتوى المتوقع 10 * 6 = 60 chars (\uXXXX = 6 chars with double slash)
    // لذلك نتأكد طول المحتوى على الأقل 60
    if (strlen(buffer) < 60) return 0;

    // التحقق من وجود 10 تكرارات لنمط \uXXXX
    int count = 0;
    for (size_t i = 0; i < strlen(buffer) - 5; i++) {
        if (buffer[i] == '\\' && buffer[i+1] == '\\' && buffer[i+2] == 'u') {
            // تحقق أن الأحرف التالية 4 أرقام سداسية عشرية
            int valid = 1;
            for (int j = 3; j <= 6; j++) {
                char c = buffer[i+j];
                if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                count++;
                i += 6 - 1; // تجاوز هذه المجموعة
            }
        }
    }

    return (count == 10);
}

int main() {
    char* temp_file_path = create_temp_file_with_unicode();
    if (temp_file_path == NULL) {
        printf("Test case 1: fail (temp file creation failed)\n");
        return 1;
    }

    printf("Temporary file created: %s\n", temp_file_path);

    if (check_file_exists_and_not_empty(temp_file_path)) {
        printf("Test case 1: pass (file exists and not empty)\n");
    } else {
        printf("Test case 1: fail (file missing or empty)\n");
        free(temp_file_path);
        return 1;
    }

    if (validate_unicode_content(temp_file_path)) {
        printf("Test case 1: pass (unicode content validated)\n");
    } else {
        printf("Test case 1: fail (unicode content invalid)\n");
    }

    free(temp_file_path);
    return 0;
}
