#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_last_names(const void *a, const void *b) {
    char *name1 = *(char **)a;
    char *name2 = *(char **)b;
    char *lastName1 = strrchr(name1, ' ') + 1;
    char *lastName2 = strrchr(name2, ' ') + 1;
    if (strlen(lastName1) == strlen(lastName2)) {
        return strcmp(lastName1, lastName2);
    }
    return strlen(lastName1) - strlen(lastName2);
}

char **lastNameLensort(char **names, int size) {
    qsort(names, size, sizeof(char *), compare_last_names);
    return names;
}

int assertArraysEqual(char **a, char **b, int n) {
    for (int i = 0; i < n; i++) {
        if (strcmp(a[i], b[i]) != 0) {
            printf("Mismatch at index %d: got '%s' expected '%s'\n", i, a[i], b[i]);
            return 0;
        }
    }
    return 1;
}

void runTests() {
    {
        char *names[] = {"Jennifer Figueroa", "Heather Mcgee", "Amanda Schwartz", "Nicole Yoder", "Melissa Hoffman"};
        char *expected[] = {"Heather Mcgee", "Nicole Yoder", "Melissa Hoffman", "Jennifer Figueroa", "Amanda Schwartz"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 1 failed\n");
        }
    }
    {
        char *names[] = {"John Doe", "Alice Lee", "Bob Zee", "Carol Foe"};
        char *expected[] = {"Bob Zee", "Alice Lee", "Carol Foe", "John Doe"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 2 failed\n");
        }
    }
    {
        char *names[] = {"James Bond"};
        char *expected[] = {"James Bond"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 3 failed\n");
        }
    }
    {
        char *names[] = {"Tom Jones", "Jerry Jones", "Rick Jones", "Bob Smith", "Paul Smith"};
        char *expected[] = {"Bob Smith", "Paul Smith", "Tom Jones", "Jerry Jones", "Rick Jones"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 4 failed\n");
        }
    }
    {
        char *names[] = {"A B", "C D", "E F", "G H", "I J"};
        char *expected[] = {"A B", "C D", "E F", "G H", "I J"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 5 failed\n");
        }
    }
    {
        char *names[] = {"A B", "C Def", "E Fghij", "G Hijklmn", "I J"};
        char *expected[] = {"A B", "I J", "C Def", "E Fghij", "G Hijklmn"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 6 failed\n");
        }
    }
    {
        char *names[] = {"Anna Longnamehere", "Bob Shortname", "Carl Mediumname", "Dave Longestnamehere"};
        char *expected[] = {"Bob Shortname", "Carl Mediumname", "Anna Longnamehere", "Dave Longestnamehere"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 7 failed\n");
        }
    }
    {
        char *names[] = {"Alice Wonderland", "Bob Marley", "Charlie Brown", "David Bowie", "Edward Norton"};
        char *expected[] = {"Bob Marley", "David Bowie", "Charlie Brown", "Edward Norton", "Alice Wonderland"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 8 failed\n");
        }
    }
    {
        char *names[] = {"James Brown","Elijah Davis","Noah Johnson","Liam O'Neill","Oliver Smith"};
        char *expected[] = {"James Brown","Elijah Davis","Noah Johnson","Liam O'Neill","Oliver Smith"};
        int n = sizeof(names) / sizeof(names[0]);
        lastNameLensort(names, n);
        if (!assertArraysEqual(names, expected, n)) {
            printf("Test 9 failed\n");
        }
    }
    {
        char *names[] = {"Aaron Smith", "Brad Jones", "Chris Lee", "Derek Adams", "Evan Brown"};
        char *expected[] = {"Chris Lee", "Brad Jones", "Aaron Smith", "Derek Adams", "Evan Brown"};
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
