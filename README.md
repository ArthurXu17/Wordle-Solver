# Wordle-Solver

### What is Wordle?

On the off-chance that you don't know what Wordle is, here's a quick explanation. It is a daily puzzle game produced by the New York Times where the objective is to guess a 5-letter secret word. The player has 6 guess attempts to do so, where each guess is just a valid 5-letter word. On each guess, the game provides feedback by telling if you if you guessed a letter correctly and placed in the correct spot, guessed a letter correctly but placed it in a incorrect spot, or if the letters you guessed weren't in the secret word. In the original game, letters that were placed correctly were marked green, letters guessed correctly but incorrectly placed are marked yellow, and all other letters are marked grey. In my version, I decided to have letters marked as uppercase for correctly placed letters, lowercase for incorrectly placed letters, and returned periods for all other letters. 

### What did I create?

Since the list of all valid secret Wordle words is publicly avaliable, I was able to develop a program that loops through all possible guesses and determine which guess results in the lowest expected number of remaining possible solutions. Afterwards, I simply applied my algorithm to the 2000+ valid secret words and my program was able to solve Wordle Puzzles using on average 3.524 guesses.

### Understanding the Output

Consider the following example from the results pdf

![Example](/demo/Aback.JPG)

My program determined that "raise" was the best first guess. 'r', 'i', 's', and 'e' are all not contained in "Aback" so in the result they were replaced by a period. 'a' however is found on "aback", but not in the second position, so a lowercase 'a' was returned. My program then determined that "clout" was the best next guess. Similarly, 'l', 'o', 'u', and 't' are all not found in raise, but 'c' was just misplaced. My program then decided to guess "abhor". In this case, 'a' and 'b' were placed correctly so they were modified to be uppercase. Finally, on the fourth guess, my program deduced that "aback" was the only possible secret word.
