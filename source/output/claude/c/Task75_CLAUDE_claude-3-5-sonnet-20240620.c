#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    char* endptr;
    long pid = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || pid <= 0) {
        fprintf(stderr, "Invalid PID. Please provide a valid positive integer.\n");
        return 1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD)pid);
    if (hProcess == NULL) {
        fprintf(stderr, "Failed to open process with PID %ld: Error code %lu\n", pid, GetLastError());
        return 1;
    }

    if (TerminateProcess(hProcess, 1)) {
        printf("Process with PID %ld has been terminated.\n", pid);
    } else {
        fprintf(stderr, "Failed to terminate process with PID %ld: Error code %lu\n", pid, GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    CloseHandle(hProcess);
    return 0;
}