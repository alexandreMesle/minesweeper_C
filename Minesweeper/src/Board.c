#include<stdio.h>
#include<stdlib.h>

#include"Board.h"
#include"Util.h"

#define SPACE " "
#define NEW_LINE "\n"
#define FLAG "F"
#define BOMB "*"
#define HIDDEN "."

/***********************************************************************/

/* Get */

ms_cell* ms_row_get(ms_cell* cells, int col)
{
	return cells + col;
}

ms_cell* ms_matrix_get(ms_cell** cells, int row, int col)
{
	return ms_row_get(*(cells + row), col);
}

ms_cell* ms_board_get(ms_board* board, int row, int col)
{
	return ms_matrix_get(board->cells, row, col);
}

/***********************************************************************/

/* Neighbours*/

void ms_cell_update_nb_neighbours(ms_board* board, int row, int col)
{
	int i, j;
	ms_board_get(board, row, col)->nb_neighbours = 0;
	for (i = ms_max(0, row -1) ; i <= ms_min(board->nb_rows - 1, row + 1) ; i++)
		for (j = ms_max(0, col -1) ; j <= ms_min(board->nb_cols - 1, col + 1) ; j++)
			if(ms_board_get(board, i, j)->is_bombed)
				ms_board_get(board, row, col)->nb_neighbours += 1;
}

// Returns nb of bombs


int ms_board_update_nb_neighbours(ms_board* board)
{
	int i, j, nb_bombs = 0;
	for (i = 0 ; i < board->nb_rows ; i++)
		for (j = 0 ; j < board->nb_cols ; j++)
			if(!ms_board_get(board, i, j)->is_bombed)
				ms_cell_update_nb_neighbours(board, i, j);
			else
				nb_bombs++;
	return nb_bombs;
}

/***********************************************************************/

/* make */

ms_cell ms_cell_make(double bomb_ratio)
{
	ms_cell cell;
	cell.is_bombed = (double)rand()/RAND_MAX < bomb_ratio;
	cell.is_shown = 0;
	cell.is_flagged = 0;
	cell.nb_neighbours = 0;
	return cell;
}

ms_cell* ms_row_make(int nb_cols, double bomb_ratio)
{
	int i = 0;
	ms_cell* cells = (ms_cell*)malloc(nb_cols * sizeof(ms_cell));
	while (i < nb_cols)
		*(cells + i++) = ms_cell_make(bomb_ratio);
	return cells;
}

ms_cell** ms_matrix_make(int nb_rows, int nb_cols, double bomb_ratio)
{
	int i = 0;
	ms_cell** cells = (ms_cell**)malloc(nb_rows * sizeof(ms_cell*));
	while (i < nb_rows)
		*(cells + i++) = ms_row_make(nb_cols, bomb_ratio);
	return cells;
}

ms_board* ms_board_make(int nb_cols, int nb_rows, double bomb_ratio)
{
	ms_board* board = (ms_board*)malloc(sizeof(ms_board));
	board->nb_rows = nb_rows;
	board->nb_cols = nb_cols;
	board->nb_flags = 0;
	board->nb_shown = 0;
	board->cells =  ms_matrix_make(nb_rows, nb_cols, bomb_ratio);
	board->nb_bombs = ms_board_update_nb_neighbours(board);
	return board;
}


/***********************************************************************/

/* print */

void ms_cell_print(ms_cell cell)
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

void ms_row_print(ms_cell* cells, int nb_cols)
{
	if (nb_cols > 0)
	{
		ms_cell_print(*cells);
		printf(SPACE);
		ms_row_print(cells + 1, nb_cols - 1);
	}
}

void ms_matrix_print(ms_cell** cells, int nb_rows, int nb_cols)
{
	if(nb_rows > 0)
	{
		ms_row_print(*cells, nb_cols);
		printf(NEW_LINE);
		ms_matrix_print(cells + 1, nb_rows - 1, nb_cols);
	}
}

void ms_board_print(ms_board* board)
{
	ms_matrix_print(board->cells, board->nb_rows, board->nb_cols);
	printf(NEW_LINE);
}

/***********************************************************************/

/* free */

void ms_row_free(ms_cell* cells)
{
	free(cells);
}

void ms_matrix_free(ms_cell** cells, int nb_rows)
{
	int i = 0;
	while(i < nb_rows)
		ms_row_free(*(cells + i++));
	free(cells);
}

void ms_board_free(ms_board* board)
{
	ms_matrix_free(board->cells, board->nb_rows);
	free(board);
}


