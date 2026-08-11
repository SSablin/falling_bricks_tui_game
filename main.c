#include <curses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "field.h"
#include "game.h"
#include "random.h"

#define FALL_INTERVAL 1

#define TOTAL_SHAPES 7

enum
{
    delay_duration = 1000
};

int main(void)
{
    srand(time(NULL));
    int row, col;
    char field[HEIGHT][WIDTH + 1];
    int shapes[7][4][2] = {{{-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
                           {{-1, 0}, {0, 0}, {0, 1}, {1, 0}}, {{-1, 0}, {0, 0}, {0, 1}, {0, 2}},
                           {{1, 0}, {0, 0}, {0, 1}, {0, 2}},  {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
                           {{-1, 1}, {0, 1}, {0, 0}, {1, 0}}};

    make_field(field);

    initscr();
    curs_set(0);
    getmaxyx(stdscr, row, col);

    cbreak();
    noecho();

    keypad(stdscr, 1);
    nodelay(stdscr, TRUE);

    timeout(delay_duration);

    int shapes_indexes[] = {I_BLOCK, O_BLOCK, T_BLOCK, J_BLOCK, L_BLOCK, S_BLOCK, Z_BLOCK};
    fisher_yates(shapes_indexes, TOTAL_SHAPES);
    size_t cur_shape_index = 0;

    int score = 0;

    while (1)
    {
        int random_shape_index = shapes_indexes[cur_shape_index];
        int cur_shape[4][2];
        memcpy(cur_shape, shapes[random_shape_index], sizeof(shapes[random_shape_index]));

        int move_x = 0, move_y = 0;

        if (!can_move(field, cur_shape, move_x, move_y))
        {
            printf("Your score: %d\n", score);
            break;
        }
        paste_block(field, cur_shape);

        time_t last_fall = time(NULL);
        int fall_interval = 1;

        while (1)
        {
            for (size_t i = 0; i < HEIGHT; i++)
            {
                move(row / 10 + i, (col - WIDTH - 1) / 2);
                addstr(field[i]);
            }

            refresh();

            int key = getch();
            switch (key)
            {
            case 'k':
            case 'w':
            case KEY_UP:
                clean_block(field, cur_shape, move_x, move_y);
                int tmp[4][2];
                memcpy(tmp, cur_shape, sizeof(cur_shape));
                rotate_block(cur_shape);

                if (!can_move(field, cur_shape, move_x, move_y))
                {
                    memcpy(cur_shape, tmp, sizeof(tmp));
                    move_block(field, cur_shape, move_x, move_y);
                    break;
                }

                move_block(field, cur_shape, move_x, move_y);
                break;
            case 'j':
            case 's':
            case KEY_DOWN:
                clean_block(field, cur_shape, move_x, move_y);
                if (!can_move(field, cur_shape, move_x, move_y + 1))
                {
                    move_block(field, cur_shape, move_x, move_y);
                    break;
                }
                move_y += 1;
                move_block(field, cur_shape, move_x, move_y);
                break;
            case 'h':
            case 'a':
            case KEY_LEFT:
                clean_block(field, cur_shape, move_x, move_y);
                if (!can_move(field, cur_shape, move_x - 2, move_y))
                {
                    move_block(field, cur_shape, move_x, move_y);
                    break;
                }
                move_x -= 2;
                move_block(field, cur_shape, move_x, move_y);
                break;
            case 'l':
            case 'd':
            case KEY_RIGHT:
                clean_block(field, cur_shape, move_x, move_y);
                if (!can_move(field, cur_shape, move_x + 2, move_y))
                {
                    move_block(field, cur_shape, move_x, move_y);
                    break;
                }
                move_x += 2;
                move_block(field, cur_shape, move_x, move_y);
                break;
            case ' ':
                while (1)
                {
                    clean_block(field, cur_shape, move_x, move_y);
                    if (!can_move(field, cur_shape, move_x, move_y + 1))
                    {
                        move_block(field, cur_shape, move_x, move_y);
                        break;
                    }
                    move_y += 1;
                    move_block(field, cur_shape, move_x, move_y);
                }
                break;

            default:
                break;
            }

            time_t now = time(NULL);
            if (now - last_fall >= fall_interval)
            {
                clean_block(field, cur_shape, move_x, move_y);
                if (can_move(field, cur_shape, move_x, move_y + 1))
                {
                    clean_block(field, cur_shape, move_x, move_y);
                    move_y++;
                    move_block(field, cur_shape, move_x, move_y);
                }
                else
                {
                    move_block(field, cur_shape, move_x, move_y);
                    score += remove_lines(field);
                    break;
                }
                last_fall = time(NULL);
            }
        }

        cur_shape_index++;
        if (cur_shape_index == 7)
        {
            cur_shape_index = 0;
            fisher_yates(shapes_indexes, TOTAL_SHAPES);
        }
    }

    refresh();
    timeout(delay_duration);
    endwin();
    return 0;
}
