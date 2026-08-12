#ifndef __GAME__H__
#define __GAME__H__

#include "shapes.h"
#include <string.h>

#include "field.h"

#define I_BLOCK 0
#define O_BLOCK 1
#define T_BLOCK 2
#define J_BLOCK 3
#define L_BLOCK 4
#define S_BLOCK 5
#define Z_BLOCK 6

#define CELL_WIDTH 2
#define START_X (WIDTH / 2 + WIDTH % 2)
#define START_Y 0

#define MAX_Y (HEIGHT - 3)
#define MIN_Y 0
#define MIN_X 2
#define MAX_X (WIDTH - 4)

#define SCORE_PER_LEVEL 500
#define SPEED_FACTOR 0.2

void paste_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES]);

void clean_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int move_x, int move_y);

int can_move(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int move_x, int move_y);

void move_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int move_x, int move_y);

unsigned int remove_lines(char field[][WIDTH + 1]);

void rotate_block(int block[][TOTAL_COORDINATES]);

int try_move_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int *move_x, int *move_y, int dx,
                   int dy);

void render_game(char field[][WIDTH + 1], int score, int row, int col);

double get_fall_interval(int score);

int try_rotate(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int *move_x, int *move_y);

int calc_score(int lines);

#endif
