/*
 * Actions.h
 *
 *  Created on: 19 oct. 2018
 *      Author: klaus
 */

#include "Board.h"

#ifndef ACTIONS_H_
#define ACTIONS_H_

#define MS_FINE_CODE 0
#define MS_WIN_CODE 1
#define MS_LOSE_CODE -1
#define MS_CANNOT_FLAG_CODE -2
#define MS_CANNOT_OPEN_CODE -3


int ms_board_toggle_flag(ms_board* board, int row, int col);
int ms_board_open(ms_board* board, int row, int col);


#endif /* ACTIONS_H_ */
