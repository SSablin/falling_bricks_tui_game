#include <curses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "field.h"
#include "game.h"

#define FALL_INTERVAL 1

enum
{
    delay_duration = 1000
};

int main(void)
{
    int row, col;
    char field[HEIGHT][WIDTH + 1];
    int shapes[7][4][2] = {{{0, 0}, {1, 0}, {2, 0}, {3, 0}},   {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
                           {{0, 0}, {1, 0}, {0, 1}, {-1, 0}},  {{0, 0}, {0, 1}, {0, 2}, {-1, 2}},
                           {{0, 0}, {0, 1}, {0, 2}, {1, 2}},   {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
                           {{0, 0}, {-1, 0}, {-1, 1}, {-2, 1}}};

    make_field(field);

    initscr();
    curs_set(0);
    getmaxyx(stdscr, row, col);

    cbreak();
    noecho();

    keypad(stdscr, 1);
    nodelay(stdscr, TRUE);

    timeout(delay_duration);

    int i = 0;

    while (1)
    {
        int cur_shape[4][2];
        memcpy(cur_shape, shapes[i], sizeof(shapes[i]));

        int move_x = 0, move_y = 0;

        if (!can_move(field, cur_shape, move_x, move_y))
        {
            printf("Your score: %d\n", i);
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
                    remove_lines(field);
                    break;
                }
                last_fall = time(NULL);
            }
        }
        i = (i + 1) % 7;
    }

    refresh();
    timeout(delay_duration);
    endwin();
    return 0;
}
