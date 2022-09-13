#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Utility function to handle allocation failures. In this
   case we print a message and exit. */
static void allocation_failed()
{
	fprintf(stderr, "Out of memory.\n");
	exit(1);
}

/* Task 1 */
game_state_t *create_default_state()
{
	// TODO: Implement this function.
	// default 18 rows, 20 columns
	unsigned int num_rows = 18, num_cols = 20;
	// game state
	game_state_t *state = (game_state_t *)malloc(sizeof(game_state_t));

	/* Check our return value to make sure we got memory */
	if (state == NULL)
	{
		allocation_failed();
	}

	/* Now we need to initialize our data.
	Since retval->data should be able to dynamically grow,
	what do you need to do? */
	state->num_rows = num_rows;
	state->board = (char **) malloc(num_rows * sizeof(char *));
	/* Check the data attribute of our vector to make sure we got memory */
	if (state->board == NULL)
	{
		free(state); // Why is this line necessary?
		allocation_failed();
	}

	// allocate memory for board
	for (unsigned int r = 0; r < num_rows; r++)
	{
		state->board[r] = malloc(num_cols * sizeof(char));
	}

	state->num_snakes = 1;
	state->snakes = (snake_t *)malloc(sizeof(snake_t));
	/* Check the data attribute of our vector to make sure we got memory */
	if (state->snakes == NULL)
	{
		free(state); // Why is this line necessary?
		allocation_failed();
	}

	// top wall
	for (unsigned int c = 0; c < num_cols; c++)
	{
		set_board_at(state, 0, c, '#');
	}
	// side walls
	for (unsigned int r = 1; r < num_rows - 1; r++)
	{
		set_board_at(state, r, 0, '#');
		set_board_at(state, r, num_cols - 1, '#');
	}

	// bottom wall
	for (unsigned int c = 0; c < num_cols; c++)
	{
		set_board_at(state, num_rows - 1, c, '#');
	}

	// fruit
	set_board_at(state, 2, 9, '*');

	// initialize one snake
	state = initialize_snakes(state);
	return state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
	// TODO: Implement this function.
	free(state);
	for (int r = 0; r < state->num_rows; r++) 
		free(state->board[r]);
	free(state->board);
	free(state->snakes);
	return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
	// TODO: Implement this function.
	return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename)
{
	FILE *f = fopen(filename, "w");
	print_board(state, f);
	fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col)
{
	return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch)
{
	state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
	// TODO: Implement this function.
	return true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
	// TODO: Implement this function.
	return true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
	// TODO: Implement this function.
	return true;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
	// TODO: Implement this function.
	return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c)
{
	// TODO: Implement this function.
	return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
	// TODO: Implement this function.
	return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
	// TODO: Implement this function.
	return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
	// TODO: Implement this function.
	return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
	// TODO: Implement this function.
	return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
	// TODO: Implement this function.
	return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
	// TODO: Implement this function.
	return;
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
	// TODO: Implement this function.
	return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
	// TODO: Implement this function.
	return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
	// TODO: Implement this function.
	return NULL;
}
