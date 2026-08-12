#ifndef __FIELD__H__
#define __FIELD__H__

#include <stddef.h>

#define WIDTH 24
#define HEIGHT 22

#define START_LOWER_LIMIT 2
#define END_LOWER_LIMIT (WIDTH - 2)

void make_field(char field[][WIDTH + 1]);
/*
 * Rendering playground:
 * walls '<! . . .!>', floor '====', '\/\/'
 * background filled with dots ('.') and spaces (' ')
 */

#endif
