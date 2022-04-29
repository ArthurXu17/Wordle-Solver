#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "wordle.h"
#include "wordlist.h"

const double ERROR = 0.00001;

static char *num_to_word(const int *numbers, const char *word) {
    char *output = malloc(6 * sizeof(char));
    for (int i = 0; i < 5; i++) {
        if (numbers[i] == 0) {
            output[i] = '.';
        } else if (numbers[i] == 1) {
            output[i] = to_lowercase(word[i]); 
        } else {
            assert(numbers[i] == 2);
            output[i] = to_uppercase(word[i]);
        }
    }
    output[5] = '\0';
    return output;
}

// expected_value(guess, secret_word, possible_solutions) determines the expected number 
//      of possible solutions remaining for guess given the current list of possible solutions
//      and the secret word
// requires: length of guess and secret_word is 5
// time: Checks all 3^5 possible outcomes
static double expected_value(char *guess,  
                             char *possible_solutions[], double possible_solutions_len) {
    assert(guess);
    assert(possible_solutions);

    double output = 0;

    char **possible_solutions_dup = malloc(possible_solutions_len * sizeof(char *));
    for (int i = 0; i < possible_solutions_len; i++) {
        possible_solutions_dup[i] = malloc((strlen(possible_solutions[i]) + 1) * sizeof(char));
        strcpy(possible_solutions_dup[i], possible_solutions[i]);
    }

    // 0 is miss, 1 is yellow, 2 is green
    for (int a = 0; a <= 2; a++) {
        for (int b = 0; b <= 2; b++) {
            for (int c = 0; c <= 2; c++) {
                for (int d = 0; d <= 2; d++) {
                    for (int e = 0; e <= 2; e++) {
                        int guess_result[5] = {a, b, c, d, e};
                        char *result = num_to_word(guess_result, guess);
                        char *result_array[1] = {result};
                        char *guess_array[1] = {guess};
                        int num_words_remaining = 
                        find_solution(guess_array, result_array, 1,
                                      possible_solutions, possible_solutions_len,
                                      possible_solutions_dup, possible_solutions_len);
                        /*printf("%s: ", result);
                        printf("%d\n", num_words_remaining);*/
                        output += (num_words_remaining / possible_solutions_len) * num_words_remaining;
                        free(result);
                    }
                }
            }
        }
    }
    // printf("End Loop1\n");
    for (int i = 0; i < possible_solutions_len; i++) {
        free(possible_solutions_dup[i]);
    }
    free(possible_solutions_dup);
    // printf("End Loop1\n");
    return output;
}

static bool contains_word(const char *word, char *strarr[], int len) {
    assert(word);
    assert(strarr);
    for (int i = 0; i < len; i++) {
        assert(strarr[i]);
        if (strcmp(word, strarr[i]) == 0) {
            return true;
        }
    }
    return false;
}

int main(void) {

    int word_idx = -1;
    printf("Please enter a valid word number between 0 and %d\n", wordle_word_list_len - 1);
    int result = scanf("%d", &word_idx);
    assert(0 <= word_idx && word_idx <= wordle_word_list_len - 1);
    char *secret_word = wordle_word_list[word_idx];

    int num_possible_solutions = wordle_word_list_len;
    char *possible_solutions[10000] = {0};

    for (int i = 0; i < num_possible_solutions; i++) {
        possible_solutions[i] = wordle_word_list[i];
    }
    
    int guess_counter = 1;

    // raise is best first guess

    char first_guess[6] = "raise";
    char first_result[6] = ".....";
    bool first_correct = evaluate_guess(secret_word, first_guess, first_result);
    char *result_array[1] = {first_result};
    char *guess_array[1] = {first_guess};
    
    if (first_correct) {
        printf("Raise is the secret word (guessed in 1)");
    } else {
        num_possible_solutions = find_solution(guess_array, result_array, 1,
                                               possible_solutions, num_possible_solutions,
                                               possible_solutions, num_possible_solutions);
        printf("Guess: %s\n", first_guess);
        printf("Result: %s\n", first_result);
        printf("Number of remaining words: %d\n", num_possible_solutions);
        printf("Remaining Words: \n");
        for (int i = 0; i < num_possible_solutions; i++) {
            printf("%s\n", possible_solutions[i]);
        }
        guess_counter++;
    }

    while (num_possible_solutions >= 1) {
        printf("Num Possible Solutions: %d\n", num_possible_solutions);
        // for each possible word that we can guess, determine the expected value
        // of the number of words left

        // use that word as our next guess

        double min_expected_words = num_possible_solutions;
        char *next_guess = malloc(6 * sizeof(char));

        if (num_possible_solutions != 1) {
            for (int i = 0; i < wordle_word_list_len; i++) {
                double expected_words = expected_value(wordle_word_list[i], 
                                                       possible_solutions, num_possible_solutions);
                if (expected_words < min_expected_words) {
                    min_expected_words = expected_words;
                    strcpy(next_guess, wordle_word_list[i]);
                    printf("%s: ", wordle_word_list[i]);
                    printf("%f\n", expected_words);
                } else if (fabs(expected_words - min_expected_words) < ERROR &&
                           contains_word(wordle_word_list[i], possible_solutions, num_possible_solutions)) {
                    // if the expected values are equal, we prefer words that are possible solutions
                    strcpy(next_guess, wordle_word_list[i]);
                    printf("%s: ", wordle_word_list[i]);
                    printf("%f\n", expected_words);  
                }
            }

            printf("%dth guess: ", guess_counter);
            printf("%s\n", next_guess);
            printf("Expected value: %f\n", min_expected_words);
            char result[6] = ".....";

            evaluate_guess(secret_word, next_guess, result);
            char *result_array[1] = {result};
            char *guess_array[1] = {next_guess};

            num_possible_solutions = find_solution(guess_array, result_array, 1,
                                                   possible_solutions, num_possible_solutions,
                                                   possible_solutions, num_possible_solutions);
        
            if (strcmp(next_guess, secret_word) == 0) {
                printf("Answer in %d guesses: %s", guess_counter, next_guess);
                num_possible_solutions--;
            } else {
                guess_counter++;
                printf("Result: %s\n", result);
                printf("Number of remaining words: %d\n", num_possible_solutions);
                printf("Remaining Words: \n");
                for (int i = 0; i < num_possible_solutions; i++) {
                    printf("%s\n", possible_solutions[i]);
                }
            }
        } else {
            strcpy(next_guess, possible_solutions[0]);
            printf("%dth guess: ", guess_counter);
            printf("%s\n", next_guess);
            assert(strcmp(next_guess, secret_word) == 0);
            printf("Answer in %d guesses: %s", guess_counter, next_guess);
            num_possible_solutions--;
        }
        free(next_guess);
    }
}
