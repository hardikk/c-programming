#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 10
#define COLS 10
#define MOVES 4

enum { UP, DOWN, LEFT, RIGHT };

int main(void)
{
  const char available = '.';
  char table[ROWS][COLS];
  int move, letter, row, col;
  bool possible[4];
  char alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                     'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                     'U', 'V', 'W', 'X', 'Y', 'Z'};

  /* using rand() to get the next move */
  srand((unsigned) time(NULL));

  /*
   * pre-fill table with dots
   */
  for (row = 0; row < ROWS; row++) {
    for (col = 0; col < COLS; col++)
      table[row][col] = available;
  }

  /* 
   * Walk the table, populating available cells with successive letters of
   * the alphabet.
   * Each next move is selected randomly from the current possible moves.
   * Break out of the walking loop early if we get boxed in.
   *
   */
  row = 0;
  col = 0;
  for (letter = 0; letter < 26; letter++)  {
    /*make the next move*/
    table[row][col] = alphabet[letter];

    /*
     * determine possible moves
     */
    for (move = 0; move < MOVES; move++)
      possible[move] = false;

    if (row > 0 && table[row-1][col] == available)
      possible[UP] = true;

    if (row < ROWS-1 && table[row+1][col] == available)
      possible[DOWN] = true;

    if (col > 0 && table[row][col-1] == available)
      possible[LEFT] = true;

    if (col < COLS-1 && table[row][col+1] == available)
      possible[RIGHT] = true;

    /*
     * exit if there no possible moves
     */
    for (move = 0; possible[move] == false && move < MOVES; move++);

    /*no 'true' values were found*/
    if (move == MOVES)
      break;

    /*
     * get next move from the possible ones
     */
    move = -1;
    while (move < 0) {
      move = rand() % MOVES;
      if (possible[move])
        break;
      else
        move = -1;
    }

    /*
     * set up the next move
     */
    switch(move) {
      case UP:
        row--;
        break;
      case DOWN:
        row++;
        break;
      case LEFT:
        col--;
        break;
      case RIGHT:
        col++;
        break;
    }
  }  /* END for each letter */

  /*
   * Display
   */
  for (row = 0; row < ROWS; row++) {
    for (col = 0; col < COLS; col++)  {
      printf("%2c", table[row][col]);
    }
    printf("\n");
  }

  return 0;
}