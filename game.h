#ifndef __GAME__H__
#define __GAME__H__

#include <string.h>

#include "field.h"

#define I_BLOCK 0
#define O_BLOCK 1
#define T_BLOCK 2
#define J_BLOCK 3
#define L_BLOCK 4
#define S_BLOCK 5
#define Z_BLOCK 6

void paste_block(char field[][WIDTH + 1], int block[][2]);

void clean_block(char field[][WIDTH + 1], int block[][2], int move_x, int move_y);

int can_move(char field[][WIDTH + 1], int block[][2], int move_x, int move_y);

void move_block(char field[][WIDTH + 1], int block[][2], int move_x, int move_y);

void remove_lines(char field[][WIDTH + 1]);

void rotate_block(int block[][2]);

#endif
