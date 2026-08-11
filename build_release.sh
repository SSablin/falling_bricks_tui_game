#!/bin/bash

gcc -c -std=c99 -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion *.c
gcc -o app.exe *.o -lcurses
