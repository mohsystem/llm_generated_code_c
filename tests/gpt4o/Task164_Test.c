#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char first[50];
    char last[50];
} Name;

int compare(const void *a, const void *b) {
    Name *nameA = (Name *)a;
    Name *nameB = (Name *)b;
    if (strlen(nameA->last) == strlen(nameB->last)) {
        return strcmp(nameA->last, nameB->last);
    }
    return strlen(nameA->last) - strlen(nameB->last);
}

void lastNameLensort(Name names[], int n) {
    qsort(names, n, sizeof(Name), compare);
}

int assertArraysEqual(Name *a, Name *b, int n) {
    for (int i = 0; i < n; i++) {
        if (strcmp(a[i].first, b[i].first) != 0 || strcmp(a[i].last, b[i].last) != 0) {
            printf("Mismatch at index %d: got '%s %s', expected '%s %s'\n", i, a[i].first, a[i].last, b[i].first, b[i].last);
            return 0;
        }
    }
    return 1;
}

void runTests() {
    {
        Name names[] = {
            {"Jennifer", "Figueroa"},
            {"Heather", "Mcgee"},
            {"Amanda", "Schwartz"},
            {"Nicole", "Yoder"},
            {"Melissa", "Hoffman"}
        };
        Name expected[] = {
            {"Heather", "Mcgee"},
            {"Nicole", "Yoder"},
            {"Melissa", "Hoffman"},
            {"Jennifer", "Figueroa"},
            {"Amanda", "Schwartz"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 1 failed\n");

        }
    }
    {
        Name names[] = {
            {"John", "Doe"},
            {"Alice", "Lee"},
            {"Bob", "Zee"},
            {"Carol", "Foe"}
        };
        Name expected[] = {
            {"Bob", "Zee"},
            {"Alice", "Lee"},
            {"Carol", "Foe"},
            {"John", "Doe"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 2 failed\n");

        }
    }
    {
        Name names[] = {
            {"James", "Bond"}
        };
        Name expected[] = {
            {"James", "Bond"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 3 failed\n");

        }
    }
    {
        Name names[] = {
            {"Tom", "Jones"},
            {"Jerry", "Jones"},
            {"Rick", "Jones"},
            {"Bob", "Smith"},
            {"Paul", "Smith"}
        };
        Name expected[] = {
            {"Bob", "Smith"},
            {"Paul", "Smith"},
            {"Tom", "Jones"},
            {"Jerry", "Jones"},
            {"Rick", "Jones"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 4 failed\n");

        }
    }
    {
        Name names[] = {
            {"A", "B"},
            {"C", "D"},
            {"E", "F"},
            {"G", "H"},
            {"I", "J"}
        };
        Name expected[] = {
            {"A", "B"},
            {"C", "D"},
            {"E", "F"},
            {"G", "H"},
            {"I", "J"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 5 failed\n");

        }
    }
    {
        Name names[] = {
            {"A", "B"},
            {"C", "Def"},
            {"E", "Fghij"},
            {"G", "Hijklmn"},
            {"I", "J"}
        };
        Name expected[] = {
            {"A", "B"},
            {"I", "J"},
            {"C", "Def"},
            {"E", "Fghij"},
            {"G", "Hijklmn"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 6 failed\n");

        }
    }
    {
        Name names[] = {
            {"Anna", "Longnamehere"},
            {"Bob", "Shortname"},
            {"Carl", "Mediumname"},
            {"Dave", "Longestnamehere"}
        };
        Name expected[] = {
            {"Bob", "Shortname"},
            {"Carl", "Mediumname"},
            {"Anna", "Longnamehere"},
            {"Dave", "Longestnamehere"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 7 failed\n");

        }
    }
    {
        Name names[] = {
            {"Alice", "Wonderland"},
            {"Bob", "Marley"},
            {"Charlie", "Brown"},
            {"David", "Bowie"},
            {"Edward", "Norton"}
        };
        Name expected[] = {
            {"Bob", "Marley"},
            {"David", "Bowie"},
            {"Charlie", "Brown"},
            {"Edward", "Norton"},
            {"Alice", "Wonderland"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 8 failed\n");

        }
    }
    {
        Name names[] = {
            {"James", "Brown"},
            {"Elijah", "Davis"},
            {"Noah", "Johnson"},
            {"Liam", "O'Neill"},
            {"Oliver", "Smith"}
        };
        Name expected[] = {
            {"James", "Brown"},
            {"Elijah", "Davis"},
            {"Noah", "Johnson"},
            {"Liam", "O'Neill"},
            {"Oliver", "Smith"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 9 failed\n");
        }
    }
    {
        Name names[] = {
            {"Aaron", "Smith"},
            {"Brad", "Jones"},
            {"Chris", "Lee"},
            {"Derek", "Adams"},
            {"Evan", "Brown"}
        };
        Name expected[] = {
            {"Chris", "Lee"},
            {"Brad", "Jones"},
            {"Aaron", "Smith"},
            {"Derek", "Adams"},
            {"Evan", "Brown"}
        };
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 10 failed\n");

        }
    }

    printf("All tests passed.\n");
}

int main() {
    runTests();
    return 0;
}
