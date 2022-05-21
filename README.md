# Wordle-Solver

### What is Wordle?

On the off-chance that you don't know what Wordle is, here's a quick explanation. It is a daily puzzle game produced by the New York Times where the objective is to guess a 5-letter secret word. The player has 6 guess attempts to do so, where each guess is just a valid 5-letter word. On each guess, the game provides feedback by telling if you if you guessed a letter correctly and placed in the correct spot, guessed a letter correctly but placed it in a incorrect spot, or if the letters you guessed weren't in the secret word. 

### What did I create?

Since the list of all valid secret Wordle words is publicly avaliable, I was able to develop a program that loops through all possible guesses and determine which guess results in the lowest expected number of remaining possible solutions. Afterwards, I simply applied my algorithm to the 2000+ valid secret words and my program was able to solve Wordle Puzzles using on average 3.524 guesses.
