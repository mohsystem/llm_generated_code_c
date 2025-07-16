#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define PORT 8000

// مسار مجلد الرفع حسب طلبك
#define UPLOAD_DIR "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task50_Test/uploads"
#define UPLOAD_FILE_PATH UPLOAD_DIR "/uploaded_file"

static int ahc_echo(void* cls, struct MHD_Connection* connection, const char* url,
                    const char* method, const char* version, const char* upload_data,
                    size_t* upload_data_size, void** con_cls) {
    static FILE* file = NULL;

    if (strcmp(method, "POST") == 0) {
        if (*upload_data_size != 0) {
            if (file == NULL) {
                // تأكد من وجود مجلد الرفع
                struct stat st = {0};
                if (stat(UPLOAD_DIR, &st) == -1) {
                    mkdir(UPLOAD_DIR, 0700);
                }
                file = fopen(UPLOAD_FILE_PATH, "wb");
                if (file == NULL) {
                    return MHD_NO;
                }
            }
            fwrite(upload_data, 1, *upload_data_size, file);
            *upload_data_size = 0;
            return MHD_YES;
        } else {
            if (file != NULL) {
                fclose(file);
                file = NULL;
            }
            const char* success_msg = "File uploaded successfully!";
            struct MHD_Response* response = MHD_create_response_from_buffer(strlen(success_msg),
                                                   (void*)success_msg, MHD_RESPMEM_MUST_COPY);
            int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
    } else {
        const char* invalid_msg = "Invalid request";
        struct MHD_Response* response = MHD_create_response_from_buffer(strlen(invalid_msg),
                                               (void*)invalid_msg, MHD_RESPMEM_MUST_COPY);
        int ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
        MHD_destroy_response(response);
        return ret;
    }
}

int main() {
    struct MHD_Daemon* daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT,
                             NULL, NULL,
                             &ahc_echo, NULL,
                             MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "Failed to start HTTP daemon\n");
        return 1;
    }

    printf("Server started on port %d\n", PORT);
    printf("Upload your file via HTTP POST to http://localhost:%d\n", PORT);
    printf("Uploaded file will be saved to:\n  %s\n", UPLOAD_FILE_PATH);
    printf("Press Enter to stop the server...\n");

    getchar();

    MHD_stop_daemon(daemon);
    printf("Server stopped\n");

    return 0;
}
