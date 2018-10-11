#include<stdio.h>
#include<stdlib.h>

/***********************************************************************/

/* Declaration */

typedef struct cell
{
	int is_bombed, is_shown, nb_neighbours;
}dm_cell;

typedef struct grid
{
	int nb_rows, nb_cols;
	dm_cell** cells;
}dm_grid;

/***********************************************************************/

/* Cells */

dm_cell dm_cell_make(double bomb_ratio)
{
	dm_cell cell;
	cell.is_bombed = (double)rand()/RAND_MAX < bomb_ratio;
	cell.is_shown = 0;
	cell.nb_neighbours = 0;
	return cell;
}

void dm_cell_print(dm_cell cell)
{
	printf("%d", cell.nb_neighbours);
}

/***********************************************************************/

/* Rows */

dm_cell* dm_row_make(int nb_cols, double bomb_ratio)
{
	int i = 0;
	dm_cell* cells = (dm_cell*)malloc(nb_cols * sizeof(dm_cell));
	while (i < nb_cols)
	{
		*(cells + i) = dm_cell_make(bomb_ratio);
		i++;
	}
	return cells;
}

void dm_row_free(dm_cell* cells)
{
	free(cells);
}

void dm_row_print(dm_cell* cells, int nb_cols)
{
	if (nb_cols > 0)
	{
		dm_cell_print(*cells);
		printf(" ");
		dm_row_print(cells + 1, nb_cols - 1);
	}
}

dm_cell* dm_row_get(dm_cell* cells, int col)
{
	return cells + col;
}

/***********************************************************************/

/* Matrix */

dm_cell** dm_matrix_make(int nb_rows, int nb_cols, double bomb_ratio)
{
	int i = 0;
	dm_cell** cells = (dm_cell**)malloc(nb_rows * sizeof(dm_cell*));
	while (i < nb_rows)
		*(cells + i++) = dm_row_make(nb_cols, bomb_ratio);
	return cells;
}

void dm_matrix_free(dm_cell** cells, int nb_rows)
{
	int i = 0;
	while(i < nb_rows)
	{
		dm_row_free(*(cells + i));
		i++;
	}
	free(cells);
}

void dm_matrix_print(dm_cell** cells, int nb_rows, int nb_cols)
{
	if(nb_rows > 0)
	{
		dm_row_print(*cells, nb_cols);
		printf("\n");
		dm_matrix_print(cells + 1, nb_rows - 1, nb_cols);
	}
}

dm_cell* dm_matrix_get(dm_cell** cells, int row, int col)
{
	return dm_row_get(*(cells + row), col);
}

/***********************************************************************/

/* Board */

dm_grid* dm_grid_make(int nb_cols, int nb_rows, double bomb_ratio)
{
	dm_grid* grid = (dm_grid*)malloc(sizeof(dm_grid));
	grid->nb_rows = nb_rows;
	grid->nb_cols = nb_cols;
	grid->cells =  dm_matrix_make(nb_rows, nb_cols, bomb_ratio);
	return grid;
}

void dm_grid_free(dm_grid* grid)
{
	dm_matrix_free(grid->cells, grid->nb_rows);
	free(grid);
}

void dm_grid_print(dm_grid* grid)
{
	dm_matrix_print(grid->cells, grid->nb_rows, grid->nb_cols);
}

dm_cell* dm_grid_get(dm_grid* grid, int row, int col)
{
	return dm_matrix_get(grid->cells, row, col);
}

int main()
{
	dm_grid* grid = dm_grid_make(5, 5, 0.1);
	dm_grid_print(grid);
	dm_cell_print(*dm_grid_get(grid, 2, 2));
	dm_grid_free(grid);
}
