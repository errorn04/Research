#!/usr/bin/env bash

# Really shitty bash script to compile the shell
# I am lazy

i686-w64-mingw32-g++ $1.c -o $1.exe -lws2_32 -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
