#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> // access()

#include "miniz/miniz.h"

#define MAX_PATH 1024

void create_directory(const char* path) {
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(path, 0755);
#endif
}

void extractZip(const char* zipFilePath, const char* destDir) {
    mz_zip_archive zipArchive = {0};

    if (!mz_zip_reader_init_file(&zipArchive, zipFilePath, 0)) {
        printf("Error opening archive: %s\n", zipFilePath);
        return;
    }

    mz_uint numFiles = mz_zip_reader_get_num_files(&zipArchive);
    for (mz_uint i = 0; i < numFiles; ++i) {
        mz_zip_archive_file_stat fileStat;
        if (!mz_zip_reader_file_stat(&zipArchive, i, &fileStat)) {
            printf("Error reading file stat\n");
            mz_zip_reader_end(&zipArchive);
            return;
        }

        char filePath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s/%s", destDir, fileStat.m_filename);

        if (mz_zip_reader_is_file_a_directory(&zipArchive, i)) {
            create_directory(filePath);
        } else {
            if (!mz_zip_reader_extract_to_file(&zipArchive, i, filePath, 0)) {
                printf("Error extracting file: %s\n", filePath);
                mz_zip_reader_end(&zipArchive);
                return;
            }
        }
    }

    mz_zip_reader_end(&zipArchive);
}

int file_exists(const char* path) {
    return access(path, F_OK) == 0;
}

void run_test_case(const char* archive_path, const char* extract_path) {
    // نظف مجلد الاستخراج لو تريد (غير مفعّل هنا)
    // create_directory(extract_path);

    size_t len = strlen(archive_path);
    if (len > 4 && strcmp(archive_path + len - 4, ".zip") == 0) {
        extractZip(archive_path, extract_path);

    } else if (len > 4 && strcmp(archive_path + len - 4, ".tar") == 0) {
        printf("Test case (TAR: %s): FAIL not implemented\n", archive_path);
    } else {
        printf("Test case (Unsupported: %s): FAIL not implemented\n", archive_path);
    }
}

int main() {
    const char* extract_path = "/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task117_Test/extract";

    // تأكد أن مجلد الاستخراج موجود
    create_directory(extract_path);

    // فقط تَست كيسز ملفات ZIP صارمة حسب التست كيسز البايثون الأصلية
    run_test_case("/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task117_Test/testZip.zip",
                  extract_path);  // عدل حسب اسم الملف داخل ZIP

    // التست كيسز للـ tar نطبع fail not implemented
    run_test_case("/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task117_Test/testTar.tar",
                  extract_path);  // مجرد اسم متوقع (لن يتم استخراج)

    return 0;
}
