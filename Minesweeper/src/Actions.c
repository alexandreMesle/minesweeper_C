#include<stdio.h>

#include"Board.h"
#include"Util.h"
#include"Actions.h"

/***********************************************************************/

/* status */

int ms_board_status(ms_board* board, ms_cell cell)
{
	if (cell.is_shown
			&& cell.is_bombed)
		return MS_LOSE_CODE;
	if (board->nb_flags == board->nb_bombs
			&& board->nb_shown + board->nb_flags == board->nb_cols * board->nb_rows)
		return MS_WIN_CODE;
	return MS_FINE_CODE;
}

/***********************************************************************/

/* flag */

int ms_cell_toggle_flag(ms_cell* cell)
{
	if(cell->is_shown)
		return MS_CANNOT_FLAG_CODE;
	cell->is_flagged = !cell->is_flagged;
	return MS_FINE_CODE;
}

int ms_board_toggle_flag(ms_board* board, int row, int col)
{
	ms_cell* cell = ms_board_get(board, row, col);
	if (ms_cell_toggle_flag(cell))
		return MS_CANNOT_FLAG_CODE;
	board->nb_flags += (cell->is_flagged) ? 1 : -1;
	return ms_board_status(board, *cell);
}

/***********************************************************************/

/* open */

void ms_cell_show(ms_cell* cell)
{
	cell->is_shown = 1;
}

int ms_board_open_cell(ms_board*, int, int);

int ms_board_open_neighbours(ms_board* board, int row, int col)
{
	int i, j, status;
	for (i = ms_max(0, row -1) ; i <= ms_min(board->nb_rows - 1, row + 1) ; i++)
		for (j = ms_max(0, col -1) ; j <= ms_min(board->nb_cols - 1, col + 1) ; j++)
		{
			status = ms_board_open_cell(board, i, j);
			if(status != MS_FINE_CODE)
				return status;
		}
	return MS_FINE_CODE;
}

int ms_board_open_cell(ms_board* board, int row, int col)
{
	ms_cell* cell = ms_board_get(board, row, col);
	if (!cell->is_shown)
	{
		ms_cell_show(cell);
		board->nb_shown++;
		if (!cell->is_flagged
			&& !cell->is_bombed
			&& !cell->nb_neighbours)
			return ms_board_open_neighbours(board, row, col);
	}
	return ms_board_status(board, *cell);
}

int ms_board_open(ms_board* board, int row, int col)
{
	ms_cell* cell = ms_board_get(board, row, col);
	if (cell->is_flagged || cell->is_shown)
		return MS_CANNOT_OPEN_CODE;
	return ms_board_open_cell(board, row, col);
}
