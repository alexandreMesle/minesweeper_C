#include<stdio.h>
#include<stdlib.h>

#define SPACE " "
#define NEW_LINE "\n"
#define FLAG "F"
#define BOMB "*"
#define HIDDEN "."

/***********************************************************************/

int min(int, int);
int max(int, int);

/* Declarations */

typedef struct cell
{
	int is_bombed,
		is_shown,
		is_flagged,
		nb_neighbours;
}
dm_cell;

typedef struct board
{
	int nb_rows,
		nb_cols,
		nb_bombs,
		nb_flags,
		nb_shown;
	dm_cell** cells;
}
dm_board;

/***********************************************************************/

/* Get */

dm_cell* dm_row_get(dm_cell* cells, int col)
{
	return cells + col;
}

dm_cell* dm_matrix_get(dm_cell** cells, int row, int col)
{
	return dm_row_get(*(cells + row), col);
}

dm_cell* dm_board_get(dm_board* board, int row, int col)
{
	return dm_matrix_get(board->cells, row, col);
}

/***********************************************************************/

/* Neighbours*/

void dm_cell_update_nb_neighbours(dm_board* board, int row, int col)
{
	int i, j;
	dm_board_get(board, row, col)->nb_neighbours = 0;
	for (i = max(0, row -1) ; i <= min(board->nb_rows - 1, row + 1) ; i++)
		for (j = max(0, col -1) ; j <= min(board->nb_cols - 1, col + 1) ; j++)
			if(dm_board_get(board, i, j)->is_bombed)
				dm_board_get(board, row, col)->nb_neighbours += 1;
}

// Returns nb of bombs


int dm_board_update_nb_neighbours(dm_board* board)
{
	int i, j, nb_bombs = 0;
	for (i = 0 ; i < board->nb_rows ; i++)
		for (j = 0 ; j < board->nb_cols ; j++)
			if(!dm_board_get(board, i, j)->is_bombed)
				dm_cell_update_nb_neighbours(board, i, j);
			else
				nb_bombs++;
	return nb_bombs;
}

/***********************************************************************/

/* make */

dm_cell dm_cell_make(double bomb_ratio)
{
	dm_cell cell;
	cell.is_bombed = (double)rand()/RAND_MAX < bomb_ratio;
	cell.is_shown = 0;
	cell.is_flagged = 0;
	cell.nb_neighbours = 0;
	return cell;
}

dm_cell* dm_row_make(int nb_cols, double bomb_ratio)
{
	int i = 0;
	dm_cell* cells = (dm_cell*)malloc(nb_cols * sizeof(dm_cell));
	while (i < nb_cols)
		*(cells + i++) = dm_cell_make(bomb_ratio);
	return cells;
}

dm_cell** dm_matrix_make(int nb_rows, int nb_cols, double bomb_ratio)
{
	int i = 0;
	dm_cell** cells = (dm_cell**)malloc(nb_rows * sizeof(dm_cell*));
	while (i < nb_rows)
		*(cells + i++) = dm_row_make(nb_cols, bomb_ratio);
	return cells;
}

dm_board* dm_board_make(int nb_cols, int nb_rows, double bomb_ratio)
{
	dm_board* board = (dm_board*)malloc(sizeof(dm_board));
	board->nb_rows = nb_rows;
	board->nb_cols = nb_cols;
	board->nb_flags = 0;
	board->nb_shown = 0;
	board->cells =  dm_matrix_make(nb_rows, nb_cols, bomb_ratio);
	board->nb_bombs = dm_board_update_nb_neighbours(board);
	return board;
}


/***********************************************************************/

/* print */

void dm_cell_print(dm_cell cell)
{
	if (cell.is_flagged)
		printf(FLAG);
	else if (!cell.is_shown)
		printf(HIDDEN);
	else if (cell.is_bombed)
		printf(BOMB);
	else
		printf("%d", cell.nb_neighbours);
}

void dm_row_print(dm_cell* cells, int nb_cols)
{
	if (nb_cols > 0)
	{
		dm_cell_print(*cells);
		printf(SPACE);
		dm_row_print(cells + 1, nb_cols - 1);
	}
}

void dm_matrix_print(dm_cell** cells, int nb_rows, int nb_cols)
{
	if(nb_rows > 0)
	{
		dm_row_print(*cells, nb_cols);
		printf(NEW_LINE);
		dm_matrix_print(cells + 1, nb_rows - 1, nb_cols);
	}
}

void dm_board_print(dm_board* board)
{
	dm_matrix_print(board->cells, board->nb_rows, board->nb_cols);
	printf(NEW_LINE);
}

/***********************************************************************/

/* free */

void dm_row_free(dm_cell* cells)
{
	free(cells);
}

void dm_matrix_free(dm_cell** cells, int nb_rows)
{
	int i = 0;
	while(i < nb_rows)
		dm_row_free(*(cells + i++));
	free(cells);
}

void dm_board_free(dm_board* board)
{
	dm_matrix_free(board->cells, board->nb_rows);
	free(board);
}


/***********************************************************************/

/* status */

// Returns -1 if it's a bomb, 1 if has win, 0 otherwise

int dm_board_status(dm_board* board, dm_cell cell)
{
	if (cell.is_shown
			&& cell.is_bombed)
		return -1;
	if (board->nb_flags == board->nb_bombs
			&& board->nb_shown + board->nb_flags == board->nb_cols * board->nb_rows)
		return 1;
	return 0;
}

/***********************************************************************/

/* flag */

void dm_cell_flag(dm_cell* cell)
{
	cell->is_flagged = 1;
}

int dm_board_flag(dm_board* board, int row, int col)
{
	dm_cell* cell = dm_board_get(board, row, col);
	dm_cell_flag(cell);
	board->nb_flags++;
	return dm_board_status(board, *cell);
}

/***********************************************************************/

/* open */


void dm_cell_show(dm_cell* cell)
{
	cell->is_shown = 1;
}

int dm_board_open_neighbours(dm_board*, int, int);

int dm_board_open(dm_board* board, int row, int col)
{
	dm_cell* cell = dm_board_get(board, row, col);
	dm_cell_show(cell);
	board->nb_shown++;
	if (!cell->is_shown
			&& !cell->is_flagged
			&& !cell->is_bombed
			&& cell->nb_neighbours == 0)
			return dm_board_open_neighbours(board, row, col);
	return dm_board_status(board, *cell);
}

int dm_board_open_neighbours(dm_board* board, int row, int col)
{
	int i, j, status;
	for (i = max(0, row -1) ; i <= min(board->nb_rows - 1, row + 1) ; i++)
		for (j = max(0, col -1) ; j <= min(board->nb_cols - 1, col + 1) ; j++)
		{
			status = dm_board_open(board, i, j);
			if(status)
				return status;
		}
	return 0;
}

/***********************************************************************/

/* main */

int main()
{
	dm_board* board = dm_board_make(5, 5, 1);
	dm_board_open(board, 0, 0);
	dm_board_print(board);
	printf("%d\n", dm_board_open(board, 4, 3));
	dm_board_print(board);
	printf("%d\n", dm_board_open(board, 1, 0));
	dm_board_print(board);
	dm_board_free(board);
}
