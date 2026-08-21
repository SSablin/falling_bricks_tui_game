#include "game.h"
#include <curses.h>

void paste_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES])
{
    for (size_t i = 0; i < TOTAL_BRICKS; i++)
    {
        int x = START_X + block[i][0] * CELL_WIDTH;
        int y = block[i][1];

        field[y][x] = '[';
        field[y][x + 1] = ']';
    }
}

void clean_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int move_x, int move_y)
{
    for (size_t i = 0; i < TOTAL_BRICKS; i++)
    {
        int x = START_X + (block[i][0] + move_x) * CELL_WIDTH;
        int y = block[i][1] + move_y;

        field[y][x] = ' ';
        field[y][x + 1] = '.';
    }
}

void move_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int move_x, int move_y)
{
    for (size_t i = 0; i < TOTAL_BRICKS; i++)
    {
        int x = START_X + (block[i][0] + move_x) * CELL_WIDTH;
        int y = block[i][1] + move_y;

        field[y][x] = '[';
        field[y][x + 1] = ']';
    }
}

int can_move(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int move_x, int move_y)
{
    for (size_t i = 0; i < TOTAL_BRICKS; i++)
    {
        int x = START_X + (block[i][0] + move_x) * CELL_WIDTH;
        int y = block[i][1] + move_y;

        if (!(y >= MIN_Y && y <= MAX_Y))
        {
            return 0;
        }

        if (!(x >= MIN_X && x <= MAX_X))
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

static void shift_rows_down(char field[][WIDTH + 1], int row)
{
    for (size_t r = row; r > 0; r--)
    {
        // copy row entirely (WIDTH + 1)
        memcpy(field[r], field[r - 1], sizeof(field[0]));
    }

    for (size_t c = START_LOWER_LIMIT; c < END_LOWER_LIMIT; c++)
        field[0][c] = (c % 2 == 0) ? ' ' : '.';
    field[0][0] = '<';
    field[0][1] = '!';
    field[0][WIDTH - 2] = '!';
    field[0][WIDTH - 1] = '>';
}

unsigned int remove_lines(char field[][WIDTH + 1])
{
    unsigned int count = 0;

    for (size_t r = 0; r < HEIGHT - 2; r++)
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
            shift_rows_down(field, r);
            if (r > 0)
                r--;
            count++;
        }
    }

    return count;
}

void rotate_block(int block[][TOTAL_COORDINATES])
{
    for (size_t i = 0; i < TOTAL_BRICKS; i++)
    {
        int tmp = block[i][1];
        block[i][1] = block[i][0];
        block[i][0] = -tmp;
    }
}

int try_move_block(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int *move_x, int *move_y, int dx, int dy)
{
    clean_block(field, block, *move_x, *move_y);
    if (!can_move(field, block, *move_x + dx, *move_y + dy))
    {
        move_block(field, block, *move_x, *move_y);
        return 0;
    }
    else
    {
        *move_x += dx;
        *move_y += dy;
        move_block(field, block, *move_x, *move_y);
        return 1;
    }
}

void render_game(char field[][WIDTH + 1], int score, int row, int col)
{
    int start_y = (row > HEIGHT + 1) ? (row - HEIGHT - 1) / 2 : 0;
    int start_x = (col > WIDTH + 1) ? (col - WIDTH - 1) / 2 : 0;
    for (size_t i = 0; i < HEIGHT; i++)
    {
        move(start_y + i, start_x);
        addstr(field[i]);
    }
    if (start_y > 0)
        mvprintw(start_y - 1, start_x, "Score: %d", score);

    refresh();
}

double get_fall_interval(int score)
{
    // level-up every n points
    int level = score / SCORE_PER_LEVEL;

    // Max speed: not faster, then 0.1 second
    double interval = 1.0 / (1.0 + level * SPEED_FACTOR);
    return interval;
}

int try_rotate(char field[][WIDTH + 1], int block[][TOTAL_COORDINATES], int *move_x, int *move_y)
{
    int backup[TOTAL_BRICKS][TOTAL_COORDINATES];
    memcpy(backup, block, sizeof(backup));
    int old_x = *move_x;
    int old_y = *move_y;

    clean_block(field, block, *move_x, *move_y);

    rotate_block(block);

    // vertical correction
    int min_y = 0;
    for (int i = 0; i < TOTAL_BRICKS; i++)
    {
        int current_y = block[i][1] + *move_y;
        if (current_y < 0)
        {
            min_y = current_y;
        }
    }
    if (min_y < 0)
    {
        *move_y -= min_y;
    }

    // Wall Kick
    int shift_x = 0;
    for (int i = 0; i < TOTAL_BRICKS; i++)
    {
        int current_x = START_X + (block[i][0] + *move_x) * CELL_WIDTH;

        if (current_x < MIN_X)
        {
            int need_shift = MIN_X - current_x;
            if (need_shift > shift_x)
            {
                shift_x = need_shift;
            }
        }
        else if (current_x > MAX_X)
        {
            int need_shift = MAX_X - current_x;
            if (need_shift < shift_x)
            {
                shift_x = need_shift;
            }
        }
    }
    *move_x += shift_x / CELL_WIDTH;

    if (!can_move(field, block, *move_x, *move_y))
    {
        memcpy(block, backup, sizeof(backup));
        *move_x = old_x;
        *move_y = old_y;
        move_block(field, block, *move_x, *move_y);
        return 0;
    }

    move_block(field, block, *move_x, *move_y);
    return 1;
}

int calc_score(int lines)
{
    static const int line_scores[] = {0, 100, 300, 500, 800};
    if (lines > 0 && lines <= 4)
        return line_scores[lines];
    return 0;
}
