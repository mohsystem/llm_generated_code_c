#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define WORD_LENGTH 6

typedef struct {
    // This is Master's API interface.
    // You should not implement it, or speculate about its implementation
    int (*guess)(char* word);
} Master;

int H[MAX_WORDS][MAX_WORDS];

int calculateMatches(const char* word1, const char* word2) {
    int matches = 0;
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (word1[i] == word2[i]) {
            matches++;
        }
    }
    return matches;
}

int findBestGuess(char** words, int* possible, int possibleSize) {
    int minMaxGroupSize = possibleSize;
    int ans = -1;
    for (int i = 0; i < possibleSize; i++) {
        int groups[7] = {0};
        for (int j = 0; j < possibleSize; j++) {
            if (i != j) {
                groups[H[possible[i]][possible[j]]]++;  // Group words by their match count with the guess
            }
        }
        int maxGroupSize = 0;
        for (int k = 0; k < 7; k++) {
            if (groups[k] > maxGroupSize) {
                maxGroupSize = groups[k];
            }
        }
        if (maxGroupSize < minMaxGroupSize) {
            minMaxGroupSize = maxGroupSize;
            ans = possible[i];  // Choose the word that minimizes the maximum group size
        }
    }
    return ans;
}

// Guess function for the Master struct
int masterGuess(char* word) {
    static char* secret = "acckzz";  // Simulated secret word (this should be replaced with the actual secret word)
    int matches = 0;
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (word[i] == secret[i]) {
            matches++;
        }
    }
    return matches;  // Return the number of exact matches
}

void findSecretWord(char** words, int wordsSize, Master* master) {
    // Calculate the matching count for each pair of words.
    for (int i = 0; i < wordsSize; i++) {
        for (int j = i; j < wordsSize; j++) {
            H[i][j] = H[j][i] = calculateMatches(words[i], words[j]);
        }
    }

    // Initially, all words are possible candidates.
    int possible[MAX_WORDS];
    for (int i = 0; i < wordsSize; i++) {
        possible[i] = i;
    }
    int possibleSize = wordsSize;

    int matches = 0;
    while (matches < 6) {
        // Find the best guess from the remaining possible words.
        int guessIndex = findBestGuess(words, possible, possibleSize);
        char* guessWord = words[guessIndex];

        // Make a guess using the master.guess function.
        matches = master->guess(guessWord);

        // If the guess is correct, we are done.
        if (matches == 6) {
            break;
        }

        // Update the possible candidates based on the guess result.
        int newPossibleSize = 0;
        for (int i = 0; i < possibleSize; i++) {
            if (H[guessIndex][possible[i]] == matches) {
                possible[newPossibleSize++] = possible[i];
            }
        }
        possibleSize = newPossibleSize;
    }
}

// Main function for testing the implementation
int main() {
    // Define the words list
    char* words[] = {"acckzz", "ccbazz", "eiowzz", "abcczz"};
    int wordsSize = 4;

    // Create Master struct and set guess function to masterGuess
    Master master;
    master.guess = masterGuess;

    // Call the function to find the secret word
    findSecretWord(words, wordsSize, &master);

    // Output the result
    printf("You guessed the secret word correctly.\n");

    return 0;
}
