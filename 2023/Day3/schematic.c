//
// Filename:    schematic.c
// Date:        12/03/2023
// Description: Advent of Code 2023 - Day 3
// Link:        https://adventofcode.com/2023/day/3
// Author:      Jon Holland
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ROW_MAX 140
#define COL_MAX 140

struct Star
{
    int row;
    int col;
};

struct Star_part
{
    struct Star star;
    int partnum;
} star_parts[2000] = {0};

int starparts_for_symbol = 0;
int starpart_counter = 0;

// TODO: Fewer global variables (use static bools)
bool sym_NW = false;
bool sym_N = false;
bool sym_NE = false;
bool sym_W = false;
bool sym_E = false;
bool sym_SW = false;
bool sym_S = false;
bool sym_SE = false;

bool star_NW = false;
bool star_N = false;
bool star_NE = false;
bool star_W = false;
bool star_E = false;
bool star_SW = false;
bool star_S = false;
bool star_SE = false;

bool shared_sym = false;
bool has_shared_star = false;
bool found_num = false;
int digit_counter = 0;
char digits[5] = {0};
size_t sum = 0;
size_t gear_sum = 0;

// Forward function declarations
bool has_collision();
bool has_shared_sym(char *curr_str, int pos);
void add_partnum(int row, int col, bool is_star);
void clear_digits_array();
void reset_collision_flags();
bool has_star_status();

int main()
{
    // Open input file for reading
    const char *fn = "input";
    FILE *fp = fopen(fn, "r");

    char line[142] = {0};
    struct Star s = {.row = -1, .col = -1};

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
        bool check_prev = false;
        bool check_next = false;
        found_num = false;
        digit_counter = 0;

        // Should we check the previous line?
        if (i > 0)
        {
            prev_str = schematic[i - 1];
            check_prev = true;
        }

        // Should we check the next line?
        if (i < (ROW_MAX - 1))
        {
            next_str = schematic[i + 1];
            check_next = true;
        }

        // Loop through current line one character at a time.
        for (int pos = 0; pos < COL_MAX; pos++)
        {
            if (isdigit(curr_str[pos]))
            {
                found_num = true;
                digits[digit_counter] = curr_str[pos];

                // Check above and/or below the number for symbols
                if (check_prev)
                {
                    if (!sym_N)
                    {
                        char c = prev_str[pos]; 
                        sym_N = (c != '.');

                        if (c == '*' && !star_N)
                        {
                            // Record position of the star
                            s.row = i - 1;
                            s.col = pos;
                            star_N = true;
                        }
                    }
                }

                if (check_next)
                {
                    if (!sym_S)
                    {
                        char c = next_str[pos];
                        sym_S = (c != '.');
                        
                        if (c == '*' && !star_S)
                        {
                            // Record position of the star
                            s.row = i + 1;
                            s.col = pos;
                            star_S = true;
                        }
                    }
                }

                digit_counter++;
            }
            else
            {
                if (!found_num)
                {
                    char c = curr_str[pos];
                    sym_W = (c != '.');
                    star_W = false;

                    if (c == '*')
                    {
                        // Record position of the star
                        s.row = i;
                        s.col = pos;
                        star_W = true;
                    }

                    // Check above and/or below the number for symbols
                    if (check_prev)
                    {
                        char c = prev_str[pos];
                        sym_NW = (c != '.');
                        star_NW = false;
                        
                        if (c == '*')
                        {
                            // Record position of the star
                            s.row = i - 1;
                            s.col = pos;
                            star_NW = true;
                        }
                    }
                    
                    if (check_next)
                    {
                        char c = next_str[pos];
                        sym_SW = (c != '.');
                        star_SW = false;

                        if (c == '*')
                        {
                            // Record position of the star
                            s.row = i + 1;
                            s.col = pos;
                            star_SW = true;
                        }
                    }
                }
                else
                {
                    char c = curr_str[pos];
                    sym_E = (c != '.');
                    star_E = false;

                    if (c == '*')
                    {
                        // Record position of the star
                        s.row = i;
                        s.col = pos;
                        star_E = true;
                    }

                    // Check above and/or below the number for symbols
                    if (check_prev)
                    {
                        char c = prev_str[pos];
                        sym_NE = (c != '.');
                        star_NE = false;

                        if (c == '*')
                        {
                            // Record position of the star
                            s.row = i - 1;
                            s.col = pos;
                            star_NE = true;
                        }
                    }
                    
                    if (check_next)
                    {
                        char c = next_str[pos];
                        sym_SE = (c != '.');
                        star_SE = false;

                        if (c == '*')
                        {
                            // Record position of the star
                            s.row = i + 1;
                            s.col = pos;
                            star_SE = true;
                        }
                    }

                    // Keep track of number if there was a symbol collision.
                    if (has_collision())
                    {
                        shared_sym = has_shared_sym(curr_str, pos);
                        add_partnum(s.row, s.col, has_star_status());

                        // Re-flag shared symbol cases.
                        if (shared_sym)
                        {
                            if (has_star_status())
                            {
                                // Need to remember the location, otherwise
                                // we won't be able to match the star in the
                                // star_parts[] array.
                                if (star_NE) star_NW = true;
                                if (star_E) star_W = true;
                                if (star_SE) star_SW = true;
                            }
                            sym_W = true;
                        }
                    }
                    else
                        clear_digits_array();

                }
            }
        }

        // Catch cases where a partnum is found at the end of a line.
        if (has_collision())
            add_partnum(s.row, s.col, has_star_status());
        else
            clear_digits_array();
    }

    printf("The sum of all the part numbers is %zu.\n", sum);
    printf("The sum of all the gear ratios is %zu.\n", gear_sum);

    fclose(fp);

    return 0;
}

bool has_collision()
{
    bool result = (sym_NW || sym_N || sym_NE ||
                   sym_W  || sym_E ||
                   sym_SW || sym_S || sym_SE ||
                   has_shared_star);

    return result;
}

void add_partnum(int spart_row, int spart_col, bool is_star)
{
    int partnum = 0;
    sscanf(digits, "%d", &partnum);
    sum += partnum;

    if (is_star)
    {
        bool has_second_partnum = false;
        int second_partnum = 0;

        // Check to see if we already found another partnum for this star.
        for (int i = 0; i < starpart_counter; i++)
        {
            if (star_parts[i].star.row == spart_row &&
                star_parts[i].star.col == spart_col)
            {
                has_second_partnum = true;
                second_partnum = star_parts[i].partnum;
                break;
            }
        }

        // If second star partnum, add it to the gear sum.
        if (partnum != 0)
        {
            if (has_second_partnum)
                gear_sum += (partnum * second_partnum);
            else
            {
                // Otherwise, just add it to the array for a potential future match.
                star_parts[starpart_counter].partnum = partnum;
                star_parts[starpart_counter].star.row = spart_row;
                star_parts[starpart_counter].star.col = spart_col;
                starpart_counter++;
            }
        }
    }

    // Clear out digits array for the next candidate
    clear_digits_array();

    found_num = false;
    reset_collision_flags();
    star_NW = star_W = star_SW = star_N = star_S = false;
    digit_counter = 0;
}

void reset_collision_flags()
{
    sym_NW = sym_N = sym_NE = false;
    sym_W = sym_E = false;
    sym_SW = sym_S = sym_SE = false;
}

void clear_digits_array()
{
    for (int c = 0; c < 5; c++) digits[c] = 0;
    digit_counter = 0;
}

bool has_shared_sym(char *curr_str, int pos)
{
    // Look ahead and see if there is an adjacent number sharing a symbol.
    return ((sym_NE || sym_E || sym_SE) && (isdigit(curr_str[pos + 1])));
}

bool has_star_status()
{
    bool result = (star_NW || star_N || star_NE ||
                   star_W  || star_E ||
                   star_SW || star_S || star_SE);

    return result;
}
