#!/bin/sh

NAME="scratchcards"
c++ -Wall -Wextra -ggdb -pedantic -std=gnu++17 -o $NAME $NAME.cpp && ./$NAME
