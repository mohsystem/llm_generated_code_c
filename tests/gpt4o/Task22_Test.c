#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "../../source/output/gpt4o/c/Task22_OPENAI_gpt-4o.c"  // Include the header for the function

//the question ask to return a list of response and this solution does return void, so give it -30%
//3 passed so give it 30% and minus 30% so final result is zero.
int main() {
   void *list1[] = { (void *)1, (void *)2, (void *)"a", (void *)"b"};
   void *list2[] = { };
   void *list3[] = {  (void *)"a", (void *)"b", (void *)"c"};
   void *list4[] = { (void *)1, (void *)2, (void *)3, (void *)4};
   void *list5[] = { (void *)1, (void *)"a", (void *)2, (void *)"b", (void *)0, (void *)15};
   void *list6[] = { (void *)5};
   void *list7[] = { (void *)"onlyString"};
   void *list8[] = { (void *)1, (void *)"2", (void *)"three", (void *)4,(void *) "5"};
   void *list9[] = { (void *)1, (void *)"-2", (void *)3, (void *)"4"};
   void *list10[] = {        (void *)100, (void *)"200", (void *)300, (void *)"400", (void *)500, (void *)"600", (void *)700, (void *)"800", (void *)900, (void *)"1000",
                (void *)1100, (void *)"1200", (void *)1300, (void *)"1400", (void *)1500, (void *)"1600", (void *)1700, (void *)"1800", (void *)1900, (void *)"2000"};


   filter_list(list1, sizeof(list1)/sizeof(list1[0]), 'i');
   filter_list(list2, sizeof(list2)/sizeof(list2[0]), 'i');
   filter_list(list3, sizeof(list3)/sizeof(list3[0]), 'i');
   filter_list(list4, sizeof(list4)/sizeof(list4[0]), 'i');
   filter_list(list5, sizeof(list5)/sizeof(list5[0]), 'i');
   filter_list(list6, sizeof(list6)/sizeof(list6[0]), 'i');
   filter_list(list7, sizeof(list7)/sizeof(list7[0]), 'i');
   filter_list(list8, sizeof(list8)/sizeof(list8[0]), 'i');
   filter_list(list9, sizeof(list9)/sizeof(list9[0]), 'i');
   filter_list(list10, sizeof(list10)/sizeof(list10[0]), 'i');
   return 0;
}

