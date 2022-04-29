// this module provides functions that can be used to play Wordle

#include <stdbool.h>

// to_uppercase(ch) returns the upper case letter of ch
// effects: none
// requires: ch is an english alphabet character
// time: O(1)
char to_uppercase(char ch);

// to_lowercase(ch) returns the lower case letter of ch
// effects: none
// requires: ch is an english alphabet character
// time: O(1)
char to_lowercase(char ch);

// all "secret" chars are only lowercase letters (a...z)
// all "guess" chars are only lowercase letters (a...z)
// all "result" chars are either:
//   lowercase letter: letter exists in secret, but not in correct position
//   UPPERCASE letter: letter in correct position
//   . (period): letter does not exist in secret word
// for each pair of guesses/results arrays: 
//   they contain the same number of strings of the same length

// evaluate_guess(secret, guess, result) returns whether or not secret is equal 
//   to guess and modifies result chars (of comparison) as described above
// note: The handling of duplicates in wordle is slightly complicated
// Examples:
//      evaluate_guess("paper", "poppy", result) => false; result = "P.P.."
//      evaluate_guess("paper", "poopy", result) => false; result = "P..p."
//      evaluate_guess("paper", "oppoy", result) => false; result = ".pP.."
//      evaluate_guess("paper", "ppooy", result) => false; result = "Pp..."
//      evaluate_guess("paper", "xpzpp", result) => false; resutl = ".p.p."
// requires: result is sufficiently large [not asserted]
//           secret and guess have the same length
// effects: result will be modified
// time: O(n)
bool evaluate_guess(const char *secret, const char *guess, char *result);

// find_solution(guesses, results, num_guesses, 
//               word_list, num_words,
//               solutions, max_solutions) returns the number of potential valid
//               secret words for a Wordle game, from word_list, that would each
//               evaluate to provided results for each of the provided guesses;
//               solutions is modified to store the subset of word_list that
//               matches the described potential secret words
//               the returned number is capped by max_solutions
// note: if find_solution returns the number n then the first n valid potential 
//       secret words from word_list is what to be stored in solutions, 
//       preserving the order.
// example: char *solutions[20] = {0};
//          char *guesses[2] = { "funky", "plane" };
//          char *results[2] = { ".....", "pla.E" };
//          find_solution(guesses, results, 2, 
//                        wordle_word_list, wordle_word_list_len, 
//                        solutions, 20) => 4
//          solutions => { "ample", "apple", "lapse", "maple" }
// requires: first "num_guesses" elements in guesses and results are valid
//           strings of the same length
//           max_solutions >= 1
//           num_guesses >= 0
//           if num_guesses > 0, then the length of the first guess must 
//           match the length of all strings in word_list
// effects: solutions may be modified
// time: O(num_words * num_guesses * m) where m is the length of each guess
int find_solution(char *guesses[], char *results[], int num_guesses,
                  char *word_list[], int num_words,
                  char *solutions[], int max_solutions);


