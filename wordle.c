
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "wordle.h"

// see wordle.h
char to_uppercase(char ch) {
  assert((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));

  if (ch >= 'A' && ch <= 'Z') {
    return ch;
  } else {
    return ch - 'a' + 'A';
  }
}

// see wordle.h
char to_lowercase(char ch) {
  assert((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));

  if (ch >= 'a' && ch <= 'z') {
    return ch;
  } else {
    return ch - 'A' + 'a';
  }
}

// see wordle.h
bool evaluate_guess(const char *secret, const char *guess, char *result) {
  assert(secret);
  assert(guess);
  assert(result);
  assert(strlen(secret) == strlen(guess));
  
  int len = strlen(secret);

  // if at any point guess[i] != secret[i], switch this to false
  bool output = true;

  // array to keep track how many times each leter occurs in secret

  int letter_counter[128] = {0};

  // initially assume all guess is wrong

  for (int i = 0; i < len; i++) {
    result[i] = '.';
  }
  
  // tallying up the letters in secret

  for (int i = 0; i < len; i++) {
    int ascii_value = secret[i];
    letter_counter[ascii_value]++;
  } 

  // first find all correctly placed letters in guess

  for (int i = 0; i < len; i++) {
    int ascii_value = guess[i];
    if (secret[i] == guess[i]) {
      result[i] = to_uppercase(guess[i]);
      letter_counter[ascii_value]--;
    } else {
      output = false;
    }
  }

  // fill in any possible remaining duplicates as lowercase

  for (int i = 0; i < len; i++) {
    int ascii_value = guess[i];
    if (letter_counter[ascii_value] > 0 && result[i] != to_uppercase(guess[i])) {
      result[i] = to_lowercase(guess[i]);
      letter_counter[ascii_value]--;
    } 
  }
  //null terminate result
  result[len] = '\0';
  
  return output;
}

// see wordle.h
int find_solution(char *guesses[], char *results[], int num_guesses,
                  char *word_list[], int num_words,
                  char *solutions[], int max_solutions) {
  assert(guesses);
  assert(results);
  assert(num_guesses >= 0);
  assert(max_solutions >= 1);
  
  int counter = 0;

  for (int i = 0; i < num_words; i++) {
    bool valid_secret_word = true;
    for (int j = 0; j < num_guesses; j++) {
      // for each guess, check if evaluate_guess(secret_word, guesses[j], result) leads to results[j]

      // first make copy of results[j]
      char * result_cpy = malloc((strlen(results[j]) + 1) * sizeof(char));
      strcpy(result_cpy, results[j]);
      
      // see if the result changes
      evaluate_guess(word_list[i], guesses[j], result_cpy);
      if (strcmp(result_cpy, results[j]) != 0) {
        valid_secret_word = false;
      }
      free(result_cpy);
    }
    if (valid_secret_word) {
      solutions[counter] = word_list[i];
      counter++;
    }
  }
  
  return counter;
}
