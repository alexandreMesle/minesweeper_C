#include<stdio.h>
#include<stdlib.h>

#include "Actions.h"
#include "Util.h"

#define MS_OPEN_KEY 'O'
#define MS_FLAG_KEY 'F'

#define MS_ROW_PROMPT_FORMAT "Row (1, .., %d): "
#define MS_COL_PROMPT_FORMAT "Col (1, .., %d): "
#define MS_ACTION_PROMPT_FORMAT "Action (%c -> Open, %c -> Toggle flag) : "
#define MS_UNKNOWN_ACTION "Unknown action\n"

#define MS_WIN_MSG "You win !\n"
#define MS_LOSE_MSG "You lose !\n"
#define MS_CANNOT_FLAG_MSG "Cannot toggle flag of this cell !\n"
#define MS_CANNOT_OPEN_MSG "Cannot open this cell !\n"

#define MS_DEFAULT_NB_ROWS 10
#define MS_DEFAULT_NB_COLS 10
#define MS_DEFAULT_RATIO 0.2

/***********************************************************************/

int ms_play_once(ms_board* board)
{
	int row = 0, col = 0;
	char action = 0;
	while(1)
	{
		ms_board_print(board);
		do
		{	printf(MS_ROW_PROMPT_FORMAT, board->nb_rows);	}
		while(!(row = ms_get_int()) || row < 1 || row > board->nb_rows);
		row--;
		do
		{	printf(MS_COL_PROMPT_FORMAT, board->nb_cols);	}
		while(!(col = ms_get_int()) || col < 1 || col > board->nb_cols);
		col--;
		do
		{
			printf(MS_ACTION_PROMPT_FORMAT, MS_OPEN_KEY, MS_FLAG_KEY);
			char action = ms_get_char();
			switch (action)
			{
				case MS_OPEN_KEY : return ms_board_open(board, row, col); break;
				case MS_FLAG_KEY : return ms_board_toggle_flag(board, row, col); break;
			}
			printf(MS_UNKNOWN_ACTION);
		}
		while(action != MS_OPEN_KEY && action != MS_FLAG_KEY);
	}
}

int ms_play(ms_board* board)
{
	int status;
	do
	{
		status = ms_play_once(board);
		switch (status)
		{
			case MS_WIN_CODE : printf (MS_WIN_MSG); break;
			case MS_LOSE_CODE : printf (MS_LOSE_MSG); break;
			case MS_CANNOT_FLAG_CODE : printf (MS_CANNOT_FLAG_MSG); break;
			case MS_CANNOT_OPEN_CODE : printf (MS_CANNOT_OPEN_MSG); break;
		}
	}
	while (status != MS_WIN_CODE && status != MS_LOSE_CODE);
	ms_board_print(board);
	return status;
}

/***************************************************/

/* main */

int main(int argv, char** argc)
{
	int rows = MS_DEFAULT_NB_ROWS,
		cols = MS_DEFAULT_NB_COLS;
	float ratio = MS_DEFAULT_RATIO;
	if (argv >= 4)
	{
		rows = atoi(argc[1]);
		cols = atoi(argc[2]);
		ratio = atof(argc[3]);
	}
	ms_board* board = ms_board_make(rows, cols, ratio);
	ms_play(board);
	ms_board_free(board);
	return 0;
}
