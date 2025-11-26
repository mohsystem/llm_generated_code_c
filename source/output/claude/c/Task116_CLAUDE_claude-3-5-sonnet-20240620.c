#include <windows.h>
#include <lm.h>
#include <stdio.h>

#pragma comment(lib, "netapi32.lib")

void change_password(const wchar_t* username, const wchar_t* password) {
    NET_API_STATUS nStatus;

    nStatus = NetUserSetInfo(NULL, username, 1003, (LPBYTE)&password, NULL);

    if (nStatus == NERR_Success) {
        wprintf(L"Password changed successfully for %s\n", username);
    } else {
        wprintf(L"Failed to change password for %s. Error code: %d\n", username, nStatus);
    }
}

int main() {
    wchar_t username[256];
    wchar_t password[256];

    wprintf(L"Enter username to change password: ");
    if (fgetws(username, sizeof(username)/sizeof(wchar_t), stdin) == NULL) {
        fwprintf(stderr, L"Error reading username\n");
        return 1;
    }
    username[wcscspn(username, L"\n")] = 0; // Remove newline

    wprintf(L"Enter new password: ");
    if (fgetws(password, sizeof(password)/sizeof(wchar_t), stdin) == NULL) {
        fwprintf(stderr, L"Error reading password\n");
        return 1;
    }
    password[wcscspn(password, L"\n")] = 0; // Remove newline

    change_password(username, password);

    return 0;
}