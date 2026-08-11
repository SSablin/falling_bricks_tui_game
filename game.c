#include "game.h"

void paste_block(char field[][WIDTH + 1], int block[][2])
{
    for (size_t i = 0; i < 4; i++)
    {
        field[block[i][1]][WIDTH / 2 + block[i][0] * 2] = '[';
        field[block[i][1]][WIDTH / 2 + 1 + block[i][0] * 2] = ']';
    }
}

void clean_block(char field[][WIDTH + 1], int block[][2], int move_x, int move_y)
{
    for (size_t i = 0; i < 4; i++)
    {
        field[block[i][1] + move_y][WIDTH / 2 + block[i][0] * 2 + move_x] = ' ';
        field[block[i][1] + move_y][WIDTH / 2 + 1 + block[i][0] * 2 + move_x] = '.';
    }
}

void move_block(char field[][WIDTH + 1], int block[][2], int move_x, int move_y)
{
    for (size_t i = 0; i < 4; i++)
    {
        field[block[i][1] + move_y][WIDTH / 2 + block[i][0] * 2 + move_x] = '[';
        field[block[i][1] + move_y][WIDTH / 2 + 1 + block[i][0] * 2 + move_x] = ']';
    }
}

int can_move(char field[][WIDTH + 1], int block[][2], int move_x, int move_y)
{
    for (size_t i = 0; i < 4; i++)
    {
        int x = WIDTH / 2 + block[i][0] * 2 + move_x;
        int y = block[i][1] + move_y;

        if (!(y >= 0 && y <= HEIGHT - 3))
        {
            return 0;
        }

        if (!(x >= 2 && x <= WIDTH - 4))
        {
            return 0;
        }

        if (field[y][x] != ' ' && field[y][x] != '.')
            return 0;
        if (field[y][x + 1] != ' ' && field[y][x + 1] != '.')
            return 0;
    }
    return 1;
}

static void move_down(char field[][WIDTH + 1], int filled_row_idx)
{
    for (int r = filled_row_idx; r > 0; r--)
    {
        memcpy(field[r], field[r - 1], sizeof(field[r - 1]));
    }

    for (size_t c = START_LOWER_LIMIT; c < END_LOWER_LIMIT; c++)
    {
        if (c % 2 == 0)
            field[0][c] = ' ';
        else
            field[0][c] = '.';
    }
}

unsigned int remove_lines(char field[][WIDTH + 1])
{
    unsigned int count = 0;

    for (int r = 0; r < HEIGHT - 2; r++)
    {
        int is_filled = 1;
        for (size_t c = START_LOWER_LIMIT; c < END_LOWER_LIMIT; c++)
        {
            if (field[r][c] == ' ' || field[r][c] == '.')
            {
                is_filled = 0;
                break;
            }
        }

        if (is_filled)
        {
            move_down(field, r);
            r--;
            count++;
        }
    }

    return count;
}

void rotate_block(int block[][2])
{
    for (size_t i = 0; i < 4; i++)
    {
        int tmp = block[i][0];
        block[i][0] = block[i][1];
        block[i][1] = -tmp;
    }
}
