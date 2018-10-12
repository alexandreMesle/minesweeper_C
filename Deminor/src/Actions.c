#include<stdio.h>

#include"Board.h"
#include"Util.h"

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
