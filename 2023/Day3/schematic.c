//
// Filename:    schematic.c
// Date:        12/03/2023
// Description: Advent of Code 2023 - Day 3
// Link:        https://adventofcode.com/2023/day/3
// Author:      Jon Holland
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
typedef int BOOL;

#define ROW_MAX 140
#define COL_MAX 140

BOOL sym_NW = FALSE;
BOOL sym_N = FALSE;
BOOL sym_NE = FALSE;
BOOL sym_W = FALSE;
BOOL sym_E = FALSE;
BOOL sym_SW = FALSE;
BOOL sym_S = FALSE;
BOOL sym_SE = FALSE;
BOOL shared_sym = FALSE;

BOOL found_num = FALSE;
int digit_counter = 0;
char digits[5] = {0};
size_t sum = 0;

// Forward function declarations
BOOL has_collision();
BOOL has_shared_sym(char *curr_str, int pos);
void add_partnum();
void clear_digits_array();
void reset_collision_flags();

int main()
{
    // Open input file for reading
    const char *fn = "input";
    FILE *fp = fopen(fn, "r");
    char line[142] = {0};

    if (fp == NULL)
    {
        printf("Error: Could not open file '%s'.\n", fn);
        exit(1);
    }

    // Declare and populate our main working array.
    char *schematic[ROW_MAX] = {0};
    int j = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        schematic[j] = strdup(line);
        j++;
    }

    // Main loop
    for (int i = 0; i < ROW_MAX; i++)
    {
        char *curr_str = schematic[i];
        char *prev_str = NULL;
        char *next_str = NULL;
        BOOL check_prev = FALSE;
        BOOL check_next = FALSE;
        found_num = FALSE;
        digit_counter = 0;

        // Should we check the previous line?
        if (i > 0)
        {
            prev_str = schematic[i - 1];
            check_prev = TRUE;
        }

        // Should we check the next line?
        if (i < (ROW_MAX - 1))
        {
            next_str = schematic[i + 1];
            check_next = TRUE;
        }

        // Loop through current line one character at a time.
        for (int pos = 0; pos < COL_MAX; pos++)
        {
            if (isdigit(curr_str[pos])) {
                found_num = TRUE;
                digits[digit_counter] = curr_str[pos];

                // Check above and/or below the number for symbols
                if (check_prev) if (!sym_N) sym_N = prev_str[pos] != '.';
                if (check_next) if (!sym_S) sym_S = next_str[pos] != '.';

                digit_counter++;
            }
            else
            {
                if (!found_num)
                {
                    sym_W = curr_str[pos] != '.';

                    // Check above and/or below the number for symbols
                    if (check_prev) sym_NW = prev_str[pos] != '.';
                    if (check_next) sym_SW = next_str[pos] != '.';
                }
                else
                {
                    sym_E = curr_str[pos] != '.';

                    // Check above and/or below the number for symbols
                    if (check_prev) sym_NE = prev_str[pos] != '.';
                    if (check_next) sym_SE = next_str[pos] != '.';

                    // Keep track of number if there was a symbol collision.
                    if (has_collision())
                    {
                        shared_sym = has_shared_sym(curr_str, pos);
                        add_partnum();

                        // Re-flag shared symbol cases.
                        if (shared_sym) sym_W = TRUE;
                    }
                    else
                        clear_digits_array();

                }
            }
        }

        // Catch cases where a partnum is found at the end of a line.
        if (has_collision())
            add_partnum();
        else
            clear_digits_array();
    }

    printf("The sum of all the part numbers is %zu.\n", sum);
    fclose(fp);

    return 0;
}

BOOL has_collision()
{
    BOOL result = (sym_NW || sym_N || sym_NE ||
                   sym_W  || sym_E ||
                   sym_SW || sym_S || sym_SE);

    return result;
}

void add_partnum()
{
    int partnum = 0;
    sscanf(digits, "%d", &partnum);
//    printf("%d\n", partnum);
    sum += partnum;

    // Clear out digits array for the next candidate
    clear_digits_array();

    found_num = FALSE;
    reset_collision_flags();
    digit_counter = 0;

}

void reset_collision_flags()
{
    sym_NW = sym_N = sym_NE = FALSE;
    sym_W = sym_E = FALSE;
    sym_SW = sym_S = sym_SE = FALSE;
}

void clear_digits_array()
{
    for (int c = 0; c < 5; c++) digits[c] = 0;
    digit_counter = 0;
}

BOOL has_shared_sym(char *curr_str, int pos)
{
    // Look ahead and see if there is an adjacent number sharing a symbol.
    return ((sym_NE || sym_E || sym_SE) && (isdigit(curr_str[pos + 1])));
}
