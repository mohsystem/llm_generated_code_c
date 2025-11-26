// C
#include <stdio.h>
//This function _getch(); doesn't exist as per to the documentation https://github.com/zoelabbb/conio.h
//also, error: unknown type name ‘class’
// #include <conio.h>

int main() {
    char password[30];
    int i = 0;
    char a;

    printf("Enter password: ");
    while (1) {
        //This function _getch(); doesn't exist as per to the documentation https://github.com/zoelabbb/conio.h
        // a = _getch();
        a = ' ';//_getch();
        if (a == '\r') {
            password[i] = '\0';
            break;
        } else if (a == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i] = a;
            i++;
            printf("*");
        }
    }
    return 0;
}