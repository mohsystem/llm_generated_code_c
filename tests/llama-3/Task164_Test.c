#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Comparator: sort by last name length, then lex order of last name
int compare(const void* a, const void* b) {
    const char* str1 = *(const char**)a;
    const char* str2 = *(const char**)b;

    const char* last1 = strrchr(str1, ' ');
    last1 = (last1) ? last1 + 1 : str1;

    const char* last2 = strrchr(str2, ' ');
    last2 = (last2) ? last2 + 1 : str2;

    int len1 = strlen(last1);
    int len2 = strlen(last2);

    if (len1 != len2) {
        return len1 - len2;
    }
    return strcmp(last1, last2);
}

void lastNameLensort(char** names, int size) {
    qsort(names, size, sizeof(char*), compare);
}

int assertArraysEqual(char** arr1, char** arr2, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr1[i], arr2[i]) != 0) {
            return 0;
        }
    }
    return 1;
}

void runTest(const char* testName, char* names[], char* expected[], int size) {
    lastNameLensort(names, size);
    if (assertArraysEqual(names, expected, size)) {
        printf("%s: PASSED\n", testName);
    } else {
        printf("%s: FAILED\nExpected order:\n", testName);
        for (int i = 0; i < size; i++) printf("  %s\n", expected[i]);
        printf("Got order:\n");
        for (int i = 0; i < size; i++) printf("  %s\n", names[i]);
    }
}

int main() {
    // Test 1: various last name lengths
    char* test1[] = {
        "Jennifer Figueroa",
        "Heather Mcgee",
        "Amanda Schwartz",
        "Nicole Yoder",
        "Melissa Hoffman"
    };
    char* expected1[] = {
        "Heather Mcgee",
        "Nicole Yoder",
        "Melissa Hoffman",
        "Jennifer Figueroa",
        "Amanda Schwartz"
    };
    runTest("Test 1: Various Last Name Lengths", test1, expected1, 5);

    // Test 2: same last name length
    char* test2[] = {
        "John Doe",
        "Alice Lee",
        "Bob Zee",
        "Carol Foe"
    };
    char* expected2[] = {
        "Bob Zee",
        "Alice Lee",
        "Carol Foe",
        "John Doe"
    };
    runTest("Test 2: Same Last Name Length", test2, expected2, 4);

    // Test 3: single name
    char* test3[] = {
        "James Bond"
    };
    char* expected3[] = {
        "James Bond"
    };
    runTest("Test 3: Single Name", test3, expected3, 1);

    // Test 4: multiple names same last name length
    char* test4[] = {
        "Tom Jones",
        "Jerry Jones",
        "Rick Jones",
        "Bob Smith",
        "Paul Smith"
    };
    char* expected4[] = {
        "Bob Smith",
        "Paul Smith",
        "Jerry Jones",
        "Rick Jones",
        "Tom Jones"
    };
    runTest("Test 4: Multiple Names Same Last Name Length", test4, expected4, 5);

    // Test 5: single letter last names
    char* test5[] = {
        "A B",
        "C D",
        "E F",
        "G H",
        "I J"
    };
    char* expected5[] = {
        "A B",
        "C D",
        "E F",
        "G H",
        "I J"
    };
    runTest("Test 5: Single Letter Last Names", test5, expected5, 5);

    // Test 6: varied last name lengths
    char* test6[] = {
        "A B",
        "C Def",
        "E Fghij",
        "G Hijklmn",
        "I J"
    };
    char* expected6[] = {
        "A B",
        "I J",
        "C Def",
        "E Fghij",
        "G Hijklmn"
    };
    runTest("Test 6: Varied Last Name Lengths", test6, expected6, 5);

    // Test 7: long last names
    char* test7[] = {
        "Anna Longnamehere",
        "Bob Shortname",
        "Carl Mediumname",
        "Dave Longestnamehere"
    };
    char* expected7[] = {
        "Bob Shortname",
        "Carl Mediumname",
        "Anna Longnamehere",
        "Dave Longestnamehere"
    };
    runTest("Test 7: Long Last Names", test7, expected7, 4);

    // Test 8: varying lengths
    char* test8[] = {
        "Alice Wonderland",
        "Bob Marley",
        "Charlie Brown",
        "David Bowie",
        "Edward Norton"
    };
    char* expected8[] = {
        "Bob Marley",
        "David Bowie",
        "Charlie Brown",
        "Edward Norton",
        "Alice Wonderland"
    };
    runTest("Test 8: Varying Lengths", test8, expected8, 5);

    // Test 9: names with punctuation
    char* test9[] = {
        "James Brown",
        "Elijah Davis",
        "Noah Johnson",
        "Liam O'Neill",
        "Oliver Smith"
    };
    char* expected9[] = {
        "James Brown",
        "Elijah Davis",
        "Noah Johnson",
        "Liam O'Neill",
        "Oliver Smith"
    };
    runTest("Test 9: Names with Punctuation", test9, expected9, 5);

    // Test 10: already sorted names
    char* test10[] = {
        "Aaron Smith",
        "Brad Jones",
        "Chris Lee",
        "Derek Adams",
        "Evan Brown"
    };
    char* expected10[] = {
        "Chris Lee",
        "Brad Jones",
        "Aaron Smith",
        "Derek Adams",
        "Evan Brown"
    };
    runTest("Test 10: Already Sorted Names", test10, expected10, 5);

    return 0;
}
