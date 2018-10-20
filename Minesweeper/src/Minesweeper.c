#include<stdio.h>
#include<stdlib.h>

#include "Actions.h"
#include "Util.h"

#define MS_OPEN_KEY 'O'
#define MS_FLAG_KEY 'F'

/***********************************************************************/

int ms_play_once(ms_board* board)
{
	int row = 0, col = 0;
	char action = 0;
	while(1)
	{
		ms_board_print(board);
		do
		{	printf("Row : ");	}
		while(!(row = ms_get_int()));
		row--;
		do
		{	printf("Col : ");	}
		while(!(col = ms_get_int()));
		col--;
		do
		{
			printf("Action (%c for Open, %c for Flag) : ", MS_OPEN_KEY, MS_FLAG_KEY);
			char action = ms_get_char();
			switch (action)
			{
				case MS_OPEN_KEY : return ms_board_open(board, row, col); break;
				case MS_FLAG_KEY : return ms_board_flag(board, row, col); break;
			}
			printf("Unknown action\n");
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
	}
	while (status == MS_FINE || status == MS_CANNOT_FLAG);
	switch (status)
	{
		case MS_WIN : printf ("You win !\n"); break;
		case MS_LOSE : printf ("You lose !\n"); break;
		case MS_CANNOT_FLAG : printf ("Can't flag this cell !\n"); break;
	}
	ms_board_print(board);
	return status;
}

/***************************************************/

/* main */

int main(int argv, char** argc)
{
	int rows = 10, cols = 10;
	float ratio = 0.2;
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
