#include <stdio.h>
#include <string.h>

void reverse_string(char *str, int len) {
  for (int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
  }
  printf("\n");
  printf(str);
  printf("\n");
}

void spin_words(char *sentence) {
  char *word = strtok(sentence, " ");
  while (word != NULL) {
    if (strlen(word) >= 5) {
      reverse_string(word, strlen(word));
    }

    word = strtok(NULL, " ");
    // printf("%s ", word);
  }
}