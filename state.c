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
	state->board = (char **)malloc(num_rows * sizeof(char *));
	/* Check the data attribute of our vector to make sure we got memory */
	if (state->board == NULL)
	{
		free(state); // Why is this line necessary?
		allocation_failed();
	}

	// allocate memory for board
	for (unsigned int r = 0; r < num_rows; r++)
	{
		state->board[r] = malloc((num_cols + 1) * sizeof(char));
	}

	state->num_snakes = 1;
	state->snakes = (snake_t *)malloc(state->num_snakes * sizeof(snake_t));
	/* Check the data attribute of our vector to make sure we got memory */
	if (state->snakes == NULL)
	{
		free(state);
		allocation_failed();
	}

	// top wall
	for (unsigned int c = 0; c < num_cols; c++)
	{
		set_board_at(state, 0, c, '#');
	}
	// set null terminator for character strings
	set_board_at(state, 0, num_cols, '\0');
	// side walls
	for (unsigned int r = 1; r < num_rows - 1; r++)
	{
		set_board_at(state, r, 0, '#');
		for (unsigned int c = 1; c < num_cols - 1; c++)
		{
			set_board_at(state, r, c, ' ');
		}
		set_board_at(state, r, num_cols - 1, '#');
		// set null terminator for character strings
		set_board_at(state, r, num_cols, '\0');
	}

	// bottom wall
	for (unsigned int c = 0; c < num_cols; c++)
	{
		set_board_at(state, num_rows - 1, c, '#');
	}
	// set null terminator for character strings
	set_board_at(state, num_rows - 1, num_cols, '\0');

	// fruit
	set_board_at(state, 2, 9, '*');

	// initialize one snake
	// The tail is at row 2, column 2, and the head is at row 2, column 4.
	// delete these lines once finished initialize_snakes
	snake_t snake;
	snake.tail_row = 2, snake.tail_col = 2, snake.head_row = 2, snake.head_col = 4;
	snake.live = true;
	state->snakes[0] = snake;
	set_board_at(state, 2, 2, 'd');
	set_board_at(state, 2, 4, 'D');
	set_board_at(state, 2, 3, '>');
	//
	state = initialize_snakes(state);
	return state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
	for (int r = 0; r < state->num_rows; r++)
		free(state->board[r]);
	free(state->board);
	free(state->snakes);
	free(state);
	return;
}

/* Task 3:
This function print out the given game board to the given file pointer.
*/
void print_board(game_state_t *state, FILE *fp)
{
	// fp = fopen()
	for (int r = 0; r < state->num_rows; r++)
	{
		fprintf(fp, "%s\n", state->board[r]);
	}
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
	if (c == 'w' || c == 'a' || c == 's' || c == 'd')
	{
		return true;
	}
	return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
	// TODO: Implement this function.
	if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x')
	{
		return true;
	}
	return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
	// TODO: Implement this function.
	if (is_head(c) || is_tail(c) || c == '^' || c == '<' || c == 'v' || c == '>')
	{
		return true;
	}
	return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
	// TODO: Implement this function.
	if (c == '^')
	{
		return 'w';
	}
	else if (c == '<')
	{
		return 'a';
	}
	else if (c == 'v')
	{
		return 's';
	}
	else if (c == '>')
	{
		return 'd';
	}
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
	if (c == 'W')
	{
		return '^';
	}
	else if (c == 'A')
	{
		return '<';
	}
	else if (c == 'S')
	{
		return 'v';
	}
	else if (c == 'D')
	{
		return '>';
	}
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
	if (c == 'v' || c == 's' || c == 'S')
	{
		return cur_row + 1;
	}
	else if (c == '^' || c == 'w' || c == 'W')
	{
		return cur_row - 1;
	}
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
	if (c == '>' || c == 'd' || c == 'D')
	{
		return cur_col + 1;
	}
	else if (c == '<' || c == 'a' || c == 'A')
	{
		return cur_col - 1;
	}
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
	snake_t snake = state->snakes[snum];
	char head_dir = get_board_at(state, snake.head_row, snake.head_col);
	unsigned int next_row = get_next_row(snake.head_row, head_dir);
	unsigned int next_col = get_next_col(snake.head_col, head_dir);
	return get_board_at(state, next_row, next_col);
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
	snake_t snake = state->snakes[snum];
	char head_dir = get_board_at(state, snake.head_row, snake.head_col);
	unsigned int next_row = get_next_row(snake.head_row, head_dir);
	unsigned int next_col = get_next_col(snake.head_col, head_dir);
	set_board_at(state, next_row, next_col, head_dir);
	set_board_at(state, snake.head_row, snake.head_col, head_to_body(head_dir));
	state->snakes[snum].head_row = next_row;
	state->snakes[snum].head_col = next_col;
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
	snake_t snake = state->snakes[snum];
	char tail_dir = get_board_at(state, snake.tail_row, snake.tail_col);
	unsigned int next_row = get_next_row(snake.tail_row, tail_dir);
	unsigned int next_col = get_next_col(snake.tail_col, tail_dir);
	char body_dir = get_board_at(state, next_row, next_col);
	set_board_at(state, next_row, next_col, body_to_tail(body_dir));
	set_board_at(state, snake.tail_row, snake.tail_col, ' ');
	state->snakes[snum].tail_row = next_row;
	state->snakes[snum].tail_col = next_col;
	return;
}

/* Task 4.5
 This function should move the snakes one timestep
 according to the rules of the game.
*/
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
	// TODO: Implement this function.

	// Each snake moves one step in the direction of its head.
	for (unsigned int i = 0; i < state->num_snakes && state->snakes[i].live; i++)
	{
		// If the head crashes into the body of a snake or a wall,
		// the snake dies and stops moving. When a snake dies, the
		// head is replaced with an x.
		char next_dir = next_square(state, i);
		if (next_dir == '#' || is_snake(next_dir))
		{
			state->snakes[i].live = false;
			set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
		}

		// If the head moves into a fruit, the snake eats the fruit
		// and grows by 1 unit in length. (You can implement growing
		// by 1 unit by updating the head without updating the tail.)
		// Each time fruit is consumed, a new fruit is generated on the board.
		else if (next_dir == '*')
		{
			update_head(state, i);
			// add more food for snakes
			add_food(state);
		}
		else
		{
			update_head(state, i);
			update_tail(state, i);
		}
	}

	return;
}

/* Task 5
This function will read a game board from a file into memory.
*/
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

/* Task 6.2
	This function takes in a game board and
	creates the array of snake_t structs.
*/
game_state_t *initialize_snakes(game_state_t *state)
{
	// TODO: Implement this function.
	return state;
}
