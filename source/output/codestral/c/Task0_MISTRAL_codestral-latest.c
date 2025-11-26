#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* process_file(void* filename) {
    pthread_mutex_lock(&lock);
    printf("Processing file: %s\n", (char*)filename);
    // Add your file processing logic here
    pthread_mutex_unlock(&lock);
    free(filename);
    return NULL;
}

int main() {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir("shared_directory")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            pthread_t thread;
            char* filename = malloc(strlen(ent->d_name) + 1);
            strcpy(filename, ent->d_name);
            pthread_create(&thread, NULL, process_file, filename);
            pthread_detach(thread);
        }
        closedir(dir);
    } else {
        perror("");
        return EXIT_FAILURE;
    }
    return 0;
}
