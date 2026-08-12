#define _POSIX_C_SOURCE 200809L

#include <curses.h>
#include <string.h>
#include <time.h>

#include "field.h"
#include "game.h"
#include "random.h"
#include "shapes.h"

enum
{
    key_escape = 27
};

int main(void)
{
    srand(time(NULL));

    struct timespec last_fall;
    clock_gettime(CLOCK_MONOTONIC, &last_fall);

    char field[HEIGHT][WIDTH + 1];

    make_field(field);

    initscr();
    curs_set(0);

    int row, col;
    getmaxyx(stdscr, row, col);

    cbreak();
    noecho();
    keypad(stdscr, 1);
    nodelay(stdscr, 0);
    timeout(16);

    int shapes_indexes[] = {I_BLOCK, O_BLOCK, T_BLOCK, J_BLOCK, L_BLOCK, S_BLOCK, Z_BLOCK};
    fisher_yates(shapes_indexes, TOTAL_SHAPES);
    size_t cur_shape_index = 0;

    int score = 0;
    int quit = 0;

    while (1)
    {
        int random_shape_index = shapes_indexes[cur_shape_index];
        int cur_shape[TOTAL_BRICKS][TOTAL_COORDINATES];
        memcpy(cur_shape, shapes[random_shape_index], sizeof(shapes[random_shape_index]));

        int move_x = 0, move_y = 0;

        if (!can_move(field, cur_shape, move_x, move_y))
        {
            break;
        }
        paste_block(field, cur_shape);

        int piece_locked = 0;

        while (1)
        {
            render_game(field, score, row, col);

            int key = getch();
            if (key != ERR)
            {
                switch (key)
                {
                case 'k':
                case 'w':
                case KEY_UP:
                    try_rotate(field, cur_shape, &move_x, &move_y);
                    break;

                case 'j':
                case 's':
                case KEY_DOWN:
                    try_move_block(field, cur_shape, &move_x, &move_y, 0, 1);
                    break;

                case 'h':
                case 'a':
                case KEY_LEFT:
                    try_move_block(field, cur_shape, &move_x, &move_y, -1, 0);
                    break;

                case 'l':
                case 'd':
                case KEY_RIGHT:
                    try_move_block(field, cur_shape, &move_x, &move_y, 1, 0);
                    break;

                case ' ':
                    while (try_move_block(field, cur_shape, &move_x, &move_y, 0, 1))
                    {
                    }
                    int lines = remove_lines(field);
                    score += calc_score(lines);
                    piece_locked = 1;
                    clock_gettime(CLOCK_MONOTONIC, &last_fall);
                    break;

                case 'q':
                case key_escape:
                    quit = 1;
                    break;

                case KEY_RESIZE:
                    getmaxyx(stdscr, row, col);
                    clear();
                    render_game(field, score, row, col);
                    break;
                }
            }

            if (quit || piece_locked)
                break;

            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            double elapsed = (now.tv_sec - last_fall.tv_sec) + (now.tv_nsec - last_fall.tv_nsec) / 1e9;

            double fall_interval = get_fall_interval(score);
            if (elapsed >= fall_interval)
            {
                last_fall = now;
                if (!try_move_block(field, cur_shape, &move_x, &move_y, 0, 1))
                {
                    int lines = remove_lines(field);
                    score += calc_score(lines);
                    break;
                }
            }
        }

        if (quit)
            break;

        cur_shape_index++;
        if (cur_shape_index == TOTAL_SHAPES)
        {
            cur_shape_index = 0;
            fisher_yates(shapes_indexes, TOTAL_SHAPES);
        }
    }

    endwin();
    printf("Your score: %d\n", score);
    return 0;
}
