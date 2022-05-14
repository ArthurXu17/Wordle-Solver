
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
int find_in_list(const char *guess, char *word_list[], int num_words) {
  assert(guess);
  assert(word_list);
  assert(num_words >= 0);
  int left = 0;
  int right = num_words - 1;

  while (left <= right) {
    int mid = (left + right) / 2;
    int compare_result = strcmp(guess, word_list[mid]);
    if (compare_result == 0) {
      return mid;
    } else if (compare_result < 0) {
      // guess precedes word_list[mid] alphabetically
      right = mid - 1;
    } else {
      // guess follows word_list[mid] alphabetically
      left = mid + 1;
    }  
  }
  return -1;
}


// see wordle.h
void available_letters(char *guesses[], char *results[], int num_guesses,
                       char *alphabet) {
  assert(guesses);
  assert(results);
  assert(num_guesses >= 0);
  assert(alphabet);
  for (int i = 0; i < num_guesses; i++) {
    assert(guesses[i]);
    assert(results[i]);
    assert(strlen(guesses[i]) == strlen(results[i]));
  }

  // default set alphabet to be "abcdefghijklmnopqrstuvwxyz\0" (all lowercase)
  for (char ch = 'a'; ch <= 'z'; ch++) {
    alphabet[ch - 'a'] = ch;
  }
  alphabet[26] = '\0';

  int guess_length = strlen(guesses[0]);

  for (int i = 0; i < num_guesses; i++) {
    for (int j = 0; j < guess_length; j++) {
      // if ch = guesses[i][j] is either a yellow or green, we then know
      // that ch is contained in the secret word
      // thus we have to update alphabet to uppercase
      // if results[i][j] is a period, then ch is not contained in word,
      // update alphabet to '.'
      char ch = guesses[i][j];
      if (results[i][j] == '.') {
        alphabet[ch - 'a'] = '.';
      } else if (ch == to_lowercase(results[i][j])) {
        alphabet[ch - 'a'] = to_uppercase(ch);
      }
    }
  }
}

// contains(item, word) determines if item is contained in word
// requires: word is not null
// effects: none
// time: O(n) where n is length of word
bool contains(char item, const char *word) {
  assert(word);
  while (*word) {
    if (item == *word) {
      return true;
    }
    word++;
  }
  return false;
}

// see wordle.h
bool valid_hard_guess(char *guesses[], char *results[], int num_guesses,
                      const char *next_guess) {

  assert(guesses);
  assert(results);
  assert(next_guess);
  assert(num_guesses >= 0);
  for (int i = 0; i < num_guesses; i++) {
    assert(guesses[i]);
    assert(results[i]);
    assert(strlen(guesses[i]) == strlen(results[i]));
  }

  // assume that output is true, if any of the 4 conditions are not satisfied
  // change output to false
  int guess_length = strlen(guesses[0]);

  //    A previously correctly positioned letter must be used in the same spot
  //    in next_guess

  // O(num_guesses * m)
  for (int i = 0; i < num_guesses; i++) {
    for (int j = 0; j < guess_length; j++) {
      char ch = guesses[i][j];
      if (results[i][j] == to_uppercase(ch) && next_guess[j] != ch) {
        return false;
      } 
    }
  }

  //    A letter from the secret word that showed up in a wrong spot in 
  //       guesses cannot be used in the same spot in next_guess.

  // O(num_guesses * m)
  for (int i = 0; i < num_guesses; i++) {
    for (int j = 0; j < guess_length; j++) {
      char ch = guesses[i][j];
      if (results[i][j] == ch && next_guess[j] == ch) {
        return false;
      } 
    }
  }

  //    A letter from the secret word that showed up in a wrong spot in 
  //       guesses should be used in next_guess.
  //    A letter that was previous identified as not part of the secret word 
  //        cannot be used in next_guess.
  char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
  // O(num_guesses * m)
  available_letters(guesses, results, num_guesses, alphabet);

  // O(m)
  for (char ch = 'a'; ch <= 'z'; ch++) {
    if (alphabet[ch - 'a'] == '.' && contains(ch, next_guess)) {
      return false;
    } else if (alphabet[ch - 'a'] == to_uppercase(ch) && 
               !contains(ch, next_guess)){
      return false;
    }
  }
  
  // if passes all checks, it is valid
  return true;
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
