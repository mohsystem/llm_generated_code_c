#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// دالة لحساب طول الاسم الأخير
int lastNameLen(const char *name) {
    const char *last = strrchr(name, ' ');
    if (last != NULL) {
        return strlen(last + 1);
    }
    return 0;
}

// دالة مقارنة بين اسمين لترتيبهم حسب طول الاسم الأخير، وإذا كانوا متساوين فالترتيب أبجدياً
int cmp(const void *a, const void *b) {
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    int lenDiff = lastNameLen(*ia) - lastNameLen(*ib);
    if (lenDiff != 0) {
        return lenDiff;
    }
    return strcmp(*ia, *ib);
}

// دالة لترتيب أسماء باستخدام qsort ودالة المقارنة
void lastNameLenSort(char *names[], int n) {
    qsort(names, n, sizeof(char *), cmp);
}

// دالة اختبار تضيف كل التست كيسز اللي بعتهالك
void runTests() {
    // 1
    char *test1[] = {"Jennifer Figueroa", "Heather Mcgee", "Amanda Schwartz", "Nicole Yoder", "Melissa Hoffman"};
    char *expected1[] = {"Heather Mcgee", "Nicole Yoder", "Melissa Hoffman", "Jennifer Figueroa", "Amanda Schwartz"};
    int n1 = sizeof(test1) / sizeof(test1[0]);
    lastNameLenSort(test1, n1);
    for (int i = 0; i < n1; i++) {
        if (strcmp(test1[i], expected1[i]) != 0) {
            printf("Test 1 failed at index %d: got '%s' expected '%s'\n", i, test1[i], expected1[i]);

        }
    }

    // 2
    char *test2[] = {"John Doe", "Alice Lee", "Bob Zee", "Carol Foe"};
    char *expected2[] = {"Bob Zee", "Alice Lee", "Carol Foe", "John Doe"};
    int n2 = sizeof(test2) / sizeof(test2[0]);
    lastNameLenSort(test2, n2);
    for (int i = 0; i < n2; i++) {
        if (strcmp(test2[i], expected2[i]) != 0) {
            printf("Test 2 failed at index %d: got '%s' expected '%s'\n", i, test2[i], expected2[i]);

        }
    }

    // 3
    char *test3[] = {"James Bond"};
    char *expected3[] = {"James Bond"};
    int n3 = sizeof(test3) / sizeof(test3[0]);
    lastNameLenSort(test3, n3);
    for (int i = 0; i < n3; i++) {
        if (strcmp(test3[i], expected3[i]) != 0) {
            printf("Test 3 failed at index %d: got '%s' expected '%s'\n", i, test3[i], expected3[i]);

        }
    }

    // 4
    char *test4[] = {"Tom Jones", "Jerry Jones", "Rick Jones", "Bob Smith", "Paul Smith"};
    char *expected4[] = {"Bob Smith", "Paul Smith", "Tom Jones", "Jerry Jones", "Rick Jones"};
    int n4 = sizeof(test4) / sizeof(test4[0]);
    lastNameLenSort(test4, n4);
    for (int i = 0; i < n4; i++) {
        if (strcmp(test4[i], expected4[i]) != 0) {
            printf("Test 4 failed at index %d: got '%s' expected '%s'\n", i, test4[i], expected4[i]);

        }
    }

    // 5
    char *test5[] = {"A B", "C D", "E F", "G H", "I J"};
    char *expected5[] = {"A B", "C D", "E F", "G H", "I J"};
    int n5 = sizeof(test5) / sizeof(test5[0]);
    lastNameLenSort(test5, n5);
    for (int i = 0; i < n5; i++) {
        if (strcmp(test5[i], expected5[i]) != 0) {
            printf("Test 5 failed at index %d: got '%s' expected '%s'\n", i, test5[i], expected5[i]);

        }
    }

    // 6
    char *test6[] = {"A B", "C Def", "E Fghij", "G Hijklmn", "I J"};
    char *expected6[] = {"A B", "I J", "C Def", "E Fghij", "G Hijklmn"};
    int n6 = sizeof(test6) / sizeof(test6[0]);
    lastNameLenSort(test6, n6);
    for (int i = 0; i < n6; i++) {
        if (strcmp(test6[i], expected6[i]) != 0) {
            printf("Test 6 failed at index %d: got '%s' expected '%s'\n", i, test6[i], expected6[i]);

        }
    }

    // 7
    char *test7[] = {"Anna Longnamehere", "Bob Shortname", "Carl Mediumname", "Dave Longestnamehere"};
    char *expected7[] = {"Bob Shortname", "Carl Mediumname", "Anna Longnamehere", "Dave Longestnamehere"};
    int n7 = sizeof(test7) / sizeof(test7[0]);
    lastNameLenSort(test7, n7);
    for (int i = 0; i < n7; i++) {
        if (strcmp(test7[i], expected7[i]) != 0) {
            printf("Test 7 failed at index %d: got '%s' expected '%s'\n", i, test7[i], expected7[i]);

        }
    }

    // 8
    char *test8[] = {"Alice Wonderland", "Bob Marley", "Charlie Brown", "David Bowie", "Edward Norton"};
    char *expected8[] = {"Bob Marley", "David Bowie", "Charlie Brown", "Edward Norton", "Alice Wonderland"};
    int n8 = sizeof(test8) / sizeof(test8[0]);
    lastNameLenSort(test8, n8);
    for (int i = 0; i < n8; i++) {
        if (strcmp(test8[i], expected8[i]) != 0) {
            printf("Test 8 failed at index %d: got '%s' expected '%s'\n", i, test8[i], expected8[i]);

        }
    }

    // 9
    char *test9[] = {"James Brown","Elijah Davis","Noah Johnson","Liam O'Neill","Oliver Smith"};
    char *expected9[] = {"James Brown","Elijah Davis","Noah Johnson","Liam O'Neill","Oliver Smith"};
    int n9 = sizeof(test9) / sizeof(test9[0]);
    lastNameLenSort(test9, n9);
    for (int i = 0; i < n9; i++) {
        if (strcmp(test9[i], expected9[i]) != 0) {
            printf("Test 9 failed at index %d: got '%s' expected '%s'\n", i, test9[i], expected9[i]);

        }
    }

    // 10
    char *test10[] = {"Aaron Smith", "Brad Jones", "Chris Lee", "Derek Adams", "Evan Brown"};
    char *expected10[] = {"Chris Lee", "Brad Jones", "Aaron Smith", "Derek Adams", "Evan Brown"};
    int n10 = sizeof(test10) / sizeof(test10[0]);
    lastNameLenSort(test10, n10);
    for (int i = 0; i < n10; i++) {
        if (strcmp(test10[i], expected10[i]) != 0) {
            printf("Test 10 failed at index %d: got '%s' expected '%s'\n", i, test10[i], expected10[i]);
        }
    }

    printf("All tests passed.\n");
}

int main() {
    runTests();
    return 0;
}
