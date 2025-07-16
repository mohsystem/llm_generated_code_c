#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <zip.h>
#include <archive.h>
#include <archive_entry.h>
#include <errno.h>

int extractZip(const char* zipFile, const char* extractDir) {
    int err = 0;
    struct zip *z = zip_open(zipFile, 0, &err);
    if (!z) {
        char errbuf[1024];
        zip_error_to_str(errbuf, sizeof(errbuf), err, errno);
        fprintf(stderr, "Error opening zip file: %s\n", errbuf);
        return 1;
    }

    zip_int64_t num_entries = zip_get_num_entries(z, 0);
    for (zip_uint64_t i = 0; i < num_entries; i++) {
        struct zip_stat st;
        if (zip_stat_index(z, i, 0, &st) != 0) {
            fprintf(stderr, "Failed to get stat for entry %llu\n", i);
            continue;
        }

        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", extractDir, st.name);

        size_t name_len = strlen(st.name);
        int is_dir = (name_len > 0 && st.name[name_len - 1] == '/');

        if (is_dir) {
            mkdir(filePath, 0777);
        } else {
            struct zip_file *f = zip_fopen_index(z, i, 0);
            if (!f) {
                fprintf(stderr, "Error opening file in zip: %s\n", st.name);
                zip_close(z);
                return 1;
            }

            FILE *fileOut = fopen(filePath, "wb");
            if (!fileOut) {
                fprintf(stderr, "Error creating file: %s\n", filePath);
                zip_fclose(f);
                zip_close(z);
                return 1;
            }

            char buffer[4096];
            zip_int64_t bytes_read;
            while ((bytes_read = zip_fread(f, buffer, sizeof(buffer))) > 0) {
                fwrite(buffer, 1, bytes_read, fileOut);
            }

            fclose(fileOut);
            zip_fclose(f);
        }
    }

    zip_close(z);
    printf("Extracted ZIP file: %s\n", zipFile);
    return 0;
}

int extractTar(const char* tarFile, const char* extractDir) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    int r = archive_read_open_filename(a, tarFile, 10240);
    if (r != ARCHIVE_OK) {
        fprintf(stderr, "Failed to open TAR archive: %s\n", tarFile);
        archive_read_free(a);
        return 1;
    }

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        const char *entryPath = archive_entry_pathname(entry);
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", extractDir, entryPath);

        if (archive_entry_filetype(entry) == AE_IFDIR) {
            mkdir(fullPath, 0777);
        } else {
            FILE *fileOut = fopen(fullPath, "wb");
            if (!fileOut) {
                fprintf(stderr, "Error creating file: %s\n", fullPath);
                archive_read_free(a);
                return 1;
            }

            const void *buff;
            size_t size;
            int64_t offset;
            while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK) {
                fwrite(buff, 1, size, fileOut);
            }
            fclose(fileOut);
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    printf("Extracted TAR file: %s\n", tarFile);
    return 0;
}

int main() {
    // المسارات التي زودتنا بها
    const char *testTarPath = "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task117_Test/testTar.tar";
    const char *testZipPath = "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task117_Test/testZip.zip";
    const char *extractDir = "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task117_Test/extract";

    // إنشاء مجلد الاستخراج (تجاهل الخطأ لو موجود)
    mkdir(extractDir, 0777);

    // اختبار tar
    int resTar = extractTar(testTarPath, extractDir);
    printf("Test TAR: %s\n", (resTar == 0) ? "PASS" : "FAIL");

    // اختبار zip
    int resZip = extractZip(testZipPath, extractDir);
    printf("Test ZIP: %s\n", (resZip == 0) ? "PASS" : "FAIL");

    return 0;
}
