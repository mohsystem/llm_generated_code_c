
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//compilation error, error: 'SIGKILL' undeclared (first use in this function);
//fixed the code
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    DWORD pid = (DWORD)atoi(argv[1]);

    HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (process == NULL) {
        fprintf(stderr, "Error: %lu\n", GetLastError());
        return 1;
    }

    if (TerminateProcess(process, 1)) {
        printf("Process %lu terminated.\n", pid);
    } else {
        fprintf(stderr, "Error: %lu\n", GetLastError());
    }

    CloseHandle(process);
    return 0;
}
