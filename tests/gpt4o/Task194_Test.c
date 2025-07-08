#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char* words[] = {"acckzz", "ccbazz", "eiowzz", "abcczz"};
int wordsSize = 4;

typedef struct {
    char* secret;
    int guessCount;
} Master;

int guess(Master* master, char* word) {
    master->guessCount += 1;
    for (int i = 0; i < wordsSize; i++) {
        if (strcmp(words[i], word) == 0) {
            int matches = 0;
            for (int j = 0; j < strlen(master->secret); j++) {
                if (master->secret[j] == word[j]) {
                    matches++;
                }
            }
            return matches;
        }
    }
    return -1; // Word not found in the list
}

int matchCount(char* w1, char* w2) {
    int count = 0;
    for (int i = 0; i < strlen(w1); i++) {
        if (w1[i] == w2[i]) {
            count++;
        }
    }
    return count;
}

void findSecretWord(char* words[], int wordsSize, Master* master, int allowedGuesses) {
    srand(time(0));  // Random seed initialization for guessing
    for (int i = 0; i < allowedGuesses; i++) {
        char* guessWord = words[rand() % wordsSize];  // Randomly pick a word from the list
        int matches = guess(master, guessWord);  // Get the number of matches
        if (matches == 6) {
            printf("You guessed the secret word correctly.\n");
            return;  // Exit if the guess is correct
        }

        // Filter words based on the number of matches
        char* newWords[wordsSize];
        int newWordsSize = 0;
        for (int j = 0; j < wordsSize; j++) {
            if (matchCount(words[j], guessWord) == matches) {
                newWords[newWordsSize++] = words[j];
            }
        }

        // Update the word list and size
        for (int j = 0; j < newWordsSize; j++) {
            words[j] = newWords[j];
        }
        wordsSize = newWordsSize;
    }
    // If the secret word is not guessed in the allowed guesses
    printf("Either you took too many guesses, or you did not find the secret word.\n");
}

int main() {
    char* secret = "acckzz";  // Set the secret word
    int allowedGuesses = 10;  // Maximum number of guesses allowed
    Master master = {secret, 0};  // Create the Master struct with the secret word

    // Call the function to find the secret word
    findSecretWord(words, wordsSize, &master, allowedGuesses);

    return 0;
}
