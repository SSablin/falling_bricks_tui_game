#!/bin/bash

clang -std=c99 -Wall -Werror -fsanitize=memory -fPIE -pie -fno-omit-frame-pointer -g -lcurses *.c -o app.exe
