#!/bin/bash

clang -std=c99 -Wall -Werror -fsanitize=address -fno-omit-frame-pointer -g -lcurses *.c -o app.exe 
