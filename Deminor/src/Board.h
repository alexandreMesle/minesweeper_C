#ifndef BOARD_H_
#define BOARD_H_

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


dm_board* dm_board_make(int nb_cols, int nb_rows, double bomb_ratio);
dm_cell* dm_board_get(dm_board* board, int row, int col);
void dm_board_print(dm_board* board);
void dm_board_free(dm_board* board);

#endif
