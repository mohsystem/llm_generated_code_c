#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PATH 1024

void create_directory(const char* path) {
    CreateDirectory(path, NULL);
}

void copy_file(const char* source_path, const char* dest_path) {
    if (CopyFile(source_path, dest_path, FALSE)) {
        printf("Copied file: %s to %s\n", source_path, dest_path);
    } else {
        fprintf(stderr, "Failed to copy file: %s\n", source_path);
    }
}

int main() {
    char source_path[MAX_PATH];
    char dest_path[MAX_PATH];

    printf("Enter the path to the source file: ");
    fgets(source_path, sizeof(source_path), stdin);
    source_path[strcspn(source_path, "\n")] = 0;

    printf("Enter the destination path: ");
    fgets(dest_path, sizeof(dest_path), stdin);
    dest_path[strcspn(dest_path, "\n")] = 0;

    create_directory(dest_path);

    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(source_path, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: No files found matching %s\n", source_path);
        return 1;
    }

    do {
        if (!(find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char full_source_path[MAX_PATH];
            char full_dest_path[MAX_PATH];

            snprintf(full_source_path, sizeof(full_source_path), "%s\\%s", source_path, find_file_data.cFileName);
            snprintf(full_dest_path, sizeof(full_dest_path), "%s\\%s", dest_path, find_file_data.cFileName);

            copy_file(full_source_path, full_dest_path);
        }
    } while (FindNextFile(hFind, &find_file_data) != 0);

    FindClose(hFind);

    return 0;
}