// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
// #define MAX_LINE_LENGTH 1000
// #define MAX_FIELDS 100
//
// void processCSV(const char* filePath) {
//     FILE* file = fopen(filePath, "r");
//     if (file == NULL) {
//         printf("Error opening file: %s\n", filePath);
//         return;
//     }
//
//     char line[MAX_LINE_LENGTH];
//     char* token;
//     int fieldCount;
//
//     if (fgets(line, sizeof(line), file)) {
//         printf("CSV Header: %s", line);
//     }
//
//     while (fgets(line, sizeof(line), file)) {
//         fieldCount = 0;
//         token = strtok(line, ",");
//
//         printf("Row: ");
//         while (token != NULL && fieldCount < MAX_FIELDS) {
//             printf("%s ", token);
//             token = strtok(NULL, ",");
//             fieldCount++;
//         }
//         printf("\n");
//     }
//
//     fclose(file);
// }
//
// void writeCSVToFile(const char* filePath, const char* data) {
//     FILE* file = fopen(filePath, "w");
//     if (file == NULL) {
//         printf("Error creating file: %s\n", filePath);
//         return;
//     }
//     fputs(data, file);
//     fclose(file);
// }
//
// int main() {
//     // Test case 1 data
//     const char* tc1 = "Name,Age,Occupation\nAlice,30,Engineer\nBob,25,Designer\nCharlie,35,Teacher\n";
//
//     // Test case 2 data
//     const char* tc2 = "Product,Price,Quantity\nLaptop,1000,10\nPhone,500,25\nTablet,300,15\n";
//
//     // Test case 3 data
//     const char* tc3 = "ID,Name\n1,John Doe\n2,Jane Smith\n3,Emily Johnson\n";
//
//     // Test case 4 data
//     const char* tc4 = "Date,Amount\n2024-01-01,100\n2024-01-02,150\n2024-01-03,200\n";
//
//     // Test case 5 data
//     const char* tc5 = "City,State,Population\nNew York,NY,8419000\nLos Angeles,CA,3980000\nChicago,IL,2716000\n";
//
//     // Test case 6 data
//     const char* tc6 = "Title,Author,Year\nThe Great Gatsby,F. Scott Fitzgerald,1925\nTo Kill a Mockingbird,Harper Lee,1960\n1984,George Orwell,1949\n";
//
//     // Test case 7 data
//     const char* tc7 = "Email,Subscribed\nuser1@example.com,Yes\nuser2@example.com,No\nuser3@example.com,Yes\n";
//
//     // Test case 8 data
//     const char* tc8 = "First Name,Last Name,Age\nMichael,Jordan,58\nLeBron,James,39\nKobe,Bryant,45\n";
//
//     // Test case 9 data
//     const char* tc9 = "ID,Name,Department,Salary\n1001,James,HR,50000\n1002,Anna,IT,60000\n1003,Robert,Finance,55000\n";
//
//     // Test case 10 data
//     const char* tc10 = "Product,Price,Quantity\nLaptop,1000,10\nPhone,500,25\nTablet,300,15\n";
//
//     // File paths
//     const char* paths[] = {
//         "C:\\Users\\1\\Downloads\\tc1.csv",
//         "C:\\Users\\1\\Downloads\\tc2.csv",
//         "C:\\Users\\1\\Downloads\\tc3.csv",
//         "C:\\Users\\1\\Downloads\\tc4.csv",
//         "C:\\Users\\1\\Downloads\\tc5.csv",
//         "C:\\Users\\1\\Downloads\\tc6.csv",
//         "C:\\Users\\1\\Downloads\\tc7.csv",
//         "C:\\Users\\1\\Downloads\\tc8.csv",
//         "C:\\Users\\1\\Downloads\\tc9.csv",
//         "C:\\Users\\1\\Downloads\\tc10.csv"
//     };
//
//     const char* data[] = {tc1, tc2, tc3, tc4, tc5, tc6, tc7, tc8, tc9, tc10};
//
//     // Write data to CSV files
//     for (int i = 0; i < 10; i++) {
//         writeCSVToFile(paths[i], data[i]);
//     }
//
//     // Process each test case
//     for (int i = 0; i < 10; i++) {
//         processCSV(paths[i]);
//     }
//
//     return 0;
// }
