#ifndef BOARD_H_
#define BOARD_H_

typedef struct cell
{
	int is_bombed,
		is_shown,
		is_flagged,
		nb_neighbours;
}
ms_cell;

typedef struct board
{
	int nb_rows,
		nb_cols,
		nb_bombs,
		nb_flags,
		nb_shown;
	ms_cell** cells;
}
ms_board;


ms_board* ms_board_make(int nb_cols, int nb_rows, double bomb_ratio);
ms_cell* ms_board_get(ms_board* board, int row, int col);
void ms_board_print(ms_board* board);
void ms_board_free(ms_board* board);

#endif
