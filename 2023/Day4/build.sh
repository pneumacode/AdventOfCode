#!/bin/sh

PROJECT="scratchcards"
c++ -Wall -Wextra -ggdb -pedantic -std=gnu++17 -o $PROJECT $PROJECT.cpp && ./$PROJECT
