#include "field.h"

void make_field(char field[][WIDTH + 1])
{
    for (size_t c = START_LOWER_LIMIT; c < END_LOWER_LIMIT; c++)
    {
        field[HEIGHT - 2][c] = '=';
        if (c % 2 == 0)
        {
            field[HEIGHT - 1][c] = '\\';
        }
        else
        {
            field[HEIGHT - 1][c] = '/';
        }
    }

    field[HEIGHT - 1][0] = ' ';
    field[HEIGHT - 1][1] = ' ';
    field[HEIGHT - 1][WIDTH - 2] = ' ';
    field[HEIGHT - 1][WIDTH - 1] = ' ';


    for (size_t r = 0; r < HEIGHT - 1; r++)
    {
        field[r][0] = '<';
        field[r][1] = '!';
        field[r][WIDTH - 2] = '!';
        field[r][WIDTH - 1] = '>';
    }

    for (size_t r = 0; r < HEIGHT - 2; r++)
    {
        for (size_t c = 2; c < WIDTH - 2; c++)
        {
            if (c % 2 == 0)
            {
                field[r][c] = ' ';
            }
            else
            {
                field[r][c] = '.';
            }
        }
    }

    for (size_t r = 0; r < HEIGHT; r++)
    {
        field[r][WIDTH] = '\0';
    }
}
