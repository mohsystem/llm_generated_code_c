#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <microhttpd.h>
#include <curl/curl.h>

#define PORT 8080
#define UPLOAD_FOLDER "/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task50_Test/uploads"
#define UPLOAD_FILE_PATH UPLOAD_FOLDER "/uploaded_file"
#define TEST_FILE_PATH "/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task50_Test/testFile.txt"

struct connection_info_struct {
    FILE *fp;
    int received;
};

static int upload_file(void *cls, struct MHD_Connection *connection,
                       const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    struct connection_info_struct *con_info = *con_cls;

    if (con_info == NULL) {
        con_info = malloc(sizeof(struct connection_info_struct));
        if (con_info == NULL)
            return MHD_NO;

        struct stat st = {0};
        if (stat(UPLOAD_FOLDER, &st) == -1) {
            mkdir(UPLOAD_FOLDER, 0700);
        }

        con_info->fp = fopen(UPLOAD_FILE_PATH, "w");
        if (con_info->fp == NULL) {
            free(con_info);
            return MHD_NO;
        }
        con_info->received = 0;

        *con_cls = (void *)con_info;
        return MHD_YES;
    }

    if (*upload_data_size != 0) {
        fwrite(upload_data, *upload_data_size, 1, con_info->fp);
        con_info->received += *upload_data_size;
        *upload_data_size = 0;
        return MHD_YES;
    } else {
        fclose(con_info->fp);
        free(con_info);
        *con_cls = NULL;
        return MHD_YES;
    }
}

static int answer_to_connection(void *cls, struct MHD_Connection *connection,
                                const char *url, const char *method,
                                const char *version, const char *upload_data,
                                size_t *upload_data_size, void **con_cls)
{
    if (0 == strcmp(method, "POST") && 0 == strcmp(url, "/upload")) {
        return upload_file(cls, connection, upload_data, upload_data_size, con_cls);
    } else {
        const char *page = "<html><body>Invalid request</body></html>";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page),
                                                (void *)page, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
        MHD_destroy_response(response);
        return ret;
    }
}

void *run_server(void *arg)
{
    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT,
                                                NULL, NULL,
                                                &answer_to_connection, NULL,
                                                MHD_OPTION_END);
    if (!daemon) {
        fprintf(stderr, "Failed to start server\n");
        return NULL;
    }
    printf("Server started on port %d\n", PORT);

    // Keep running for 10 seconds to allow test client to run
    sleep(10);

    MHD_stop_daemon(daemon);
    printf("Server stopped\n");
    return NULL;
}

int test_upload()
{
    CURL *curl;
    CURLcode res;
    struct stat st;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to init curl\n");
        return 0;
    }

    // Check if test file exists
    if (stat(TEST_FILE_PATH, &st) != 0) {
        fprintf(stderr, "Test file does not exist: %s\n", TEST_FILE_PATH);
        curl_easy_cleanup(curl);
        return 0;
    }

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;

    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "file",
                 CURLFORM_FILE, TEST_FILE_PATH,
                 CURLFORM_END);

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/upload");
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    res = curl_easy_perform(curl);

    curl_formfree(formpost);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return 0;
    }

    // Check if upload file created
    if (stat(UPLOAD_FILE_PATH, &st) == 0 && st.st_size > 0) {
        printf("Test case: File upload: PASS\n");
        return 1;
    } else {
        printf("Test case: File upload: FAIL\n");
        return 0;
    }
}

int main()
{
    pthread_t server_thread;

    if (pthread_create(&server_thread, NULL, run_server, NULL) != 0) {
        fprintf(stderr, "Failed to create server thread\n");
        return 1;
    }

    // Wait a little to ensure server is up (1 second)
    sleep(1);

    // Run upload test
    int result = test_upload();

    // Wait for server thread to finish
    pthread_join(server_thread, NULL);

    if (result) {
        return 0; // success
    } else {
        return 1; // failure
    }
}
