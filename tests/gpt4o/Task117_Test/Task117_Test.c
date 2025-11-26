#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zip.h>
#include <archive.h>
#include <archive_entry.h>

void extract_zip(const char *file_path, const char *extract_to) {
    int err = 0;
    struct zip *archive = zip_open(file_path, 0, &err);
    if (!archive) {
        fprintf(stderr, "Failed to open ZIP archive\n");
        return;
    }

    zip_int64_t entries = zip_get_num_entries(archive, 0);
    for (zip_int64_t i = 0; i < entries; ++i) {
        struct zip_stat st;
        zip_stat_index(archive, i, 0, &st);

        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", extract_to, st.name);

        FILE *out_file = fopen(full_path, "wb");
        if (!out_file) {
            fprintf(stderr, "Failed to create file: %s\n", full_path);
            continue;
        }

        struct zip_file *file = zip_fopen_index(archive, i, 0);
        if (!file) {
            fprintf(stderr, "Failed to open file in archive: %s\n", st.name);
            fclose(out_file);
            continue;
        }

        char buffer[8192];
        zip_int64_t bytes_read;
        while ((bytes_read = zip_fread(file, buffer, sizeof(buffer))) > 0)
            fwrite(buffer, 1, bytes_read, out_file);

        zip_fclose(file);
        fclose(out_file);
    }
    zip_close(archive);
}

void extract_tar(const char *file_path, const char *extract_to) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;

    archive_read_support_format_tar(a);
    int r = archive_read_open_filename(a, file_path, 10240);
    if (r != ARCHIVE_OK) {
        fprintf(stderr, "Failed to open TAR archive\n");
        archive_read_free(a);
        return;
    }

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", extract_to, archive_entry_pathname(entry));
        FILE *out_file = fopen(full_path, "wb");
        if (!out_file) {
            fprintf(stderr, "Failed to create file: %s\n", full_path);
            archive_read_data_skip(a);
            continue;
        }

        const void *buff;
        size_t size;
        int64_t offset;
        while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK) {
            fwrite(buff, 1, size, out_file);
        }
        fclose(out_file);
    }
    archive_read_close(a);
    archive_read_free(a);
}

int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

void run_test_case(const char *archive_path, const char *extract_to) {
    size_t len = strlen(archive_path);

    if (len > 4 && strcmp(archive_path + len - 4, ".zip") == 0) {
        extract_zip(archive_path, extract_to);
    } else if (len > 4 && strcmp(archive_path + len - 4, ".tar") == 0) {
        extract_tar(archive_path, extract_to);
        char expected_file_path[512];
        snprintf(expected_file_path, sizeof(expected_file_path), "%s/%s", extract_to);
        if (file_exists(expected_file_path)) {
            printf("Test case (TAR: %s): PASS\n", archive_path);
        } else {
            printf("Test case (TAR: %s): FAIL\n", archive_path);
        }
    } else {
        printf("Test case (Unsupported: %s): FAIL not implemented\n", archive_path);
    }
}

int main() {
    const char *extract_dir = "/home/kali/CLionProjects/llm_generated_code_c/tests/gpt4o/Task117_Test/extract";

    run_test_case("/home/kali/CLionProjects/llm_generated_code_c/tests/gpt4o/Task117_Test/testZip.zip",
                  extract_dir);  // Replace with actual filename inside ZIP

    run_test_case("/home/kali/CLionProjects/llm_generated_code_c/tests/gpt4o/Task117_Test/testTar.tar",
                  extract_dir);  // Replace with actual filename inside TAR

    return 0;
}
