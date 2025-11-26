#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT 1024

// نسخة مبسطة من البرنامج الأصلي لتشغيله داخليًا مع إدخال نصي
int run_program_with_input(const char* input, char* output, size_t max_output) {
    FILE* in = tmpfile();
    FILE* out = tmpfile();
    if (!in || !out) {
        snprintf(output, max_output, "Failed to open temp files\n");
        return 1;
    }

    // اكتب الإدخال للملف المؤقت
    fputs(input, in);
    rewind(in);

    // احفظ stdin و stdout الأصليين
    FILE* old_stdin = stdin;
    FILE* old_stdout = stdout;
    stdin = in;
    stdout = out;

    // بداية تنفيذ البرنامج الأصلي داخليًا
    int choice;
    int num1, num2;
    double result;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
        printf("5. Exit\n");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            // تنظيف buffer
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (choice == 5) {
            break;
        }

        if (choice >= 1 && choice <= 4) {
            printf("Enter the first number: ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input. Please enter an integer.\n");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            printf("Enter the second number: ");
            if (scanf("%d", &num2) != 1) {
                printf("Invalid input. Please enter an integer.\n");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }

            if (choice == 1) result = num1 + num2;
            else if (choice == 2) result = num1 - num2;
            else if (choice == 3) result = num1 * num2;
            else if (choice == 4) {
                if (num2 == 0) {
                    printf("Error: Cannot divide by zero.\n");
                    break;
                }
                result = (double)num1 / num2;
            }

            printf("Result: %f\n", result);
        } else {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
    // نهاية البرنامج الأصلي

    fflush(stdout);
    stdout = old_stdout;
    stdin = old_stdin;

    rewind(out);
    size_t read_bytes = fread(output, 1, max_output - 1, out);
    output[read_bytes] = '\0';

    fclose(in);
    fclose(out);
    return 0;
}

int main() {
    struct {
        const char* input;
        const char* expected_output_contains; // نتحقق من وجود هذه النصوص جزئيًا لأن الناتج متغير ويحتوي قوائم وأرقام
        const char* description;
    } tests[] = {
        {
            // اختبار جمع: 1, 10, 5 ثم خروج 5
            "1\n10\n5\n5\n",
            "Result: 15.000000",
            "Test 1: Addition"
        },
        {
            // اختبار طرح: 2, 20, 3 ثم خروج 5
            "2\n20\n3\n5\n",
            "Result: 17.000000",
            "Test 2: Subtraction"
        },
        {
            // اختبار ضرب: 3, 4, 5 ثم خروج 5
            "3\n4\n5\n5\n",
            "Result: 20.000000",
            "Test 3: Multiplication"
        },
        {
            // اختبار قسمة صحيحة: 4, 20, 4 ثم خروج 5
            "4\n20\n4\n5\n",
            "Result: 5.000000",
            "Test 4: Division"
        },
        {
            // اختبار قسمة على صفر: 4, 10, 0 (ينبغي طباعة رسالة الخطأ والخروج)
            "4\n10\n0\n",
            "Error: Cannot divide by zero.",
            "Test 5: Division by zero"
        },
        {
            // اختيار خاطئ ثم اختيار صحيح للخروج
            "10\n5\n",
            "Invalid choice. Please enter a number between 1 and 5.",
            "Test 6: Invalid choice"
        },
        {
            // إدخال حرف بدل رقم في اختيار القائمة ثم خروج
            "a\n5\n",
            "Invalid choice. Please enter a number between 1 and 5.",
            "Test 7: Non-numeric menu input"
        },
        {
            // إدخال حرف بدل الرقم الأول ثم رقم صحيح ثم خروج
            "1\na\n5\n",
            "Invalid input. Please enter an integer.",
            "Test 8: Non-numeric first number"
        },
        {
            // إدخال حرف بدل الرقم الثاني ثم رقم صحيح ثم خروج
            "1\n10\na\n5\n",
            "Invalid input. Please enter an integer.",
            "Test 9: Non-numeric second number"
        },
    };

    char output[MAX_OUTPUT];
    int n = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < n; i++) {
        int ret = run_program_with_input(tests[i].input, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if (strstr(output, tests[i].expected_output_contains) != NULL) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected output to contain:\n%s\nGot:\n%s\n", tests[i].expected_output_contains, output);
        }
    }
    return 0;
}
