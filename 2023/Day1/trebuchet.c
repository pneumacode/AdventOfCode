//
// Filename:    trebuchet.c
// Date:        12/01/2023
// Description: Advent of Code 2023 - Day 1
// Link:        https://adventofcode.com/2023/day/1
// Author:      Jon Holland
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

int main()
{
    // Open input file for reading
    const char *fn = "input";
    FILE *fp = fopen(fn, "r");

    if (fp == NULL)
    {
        printf("Error: Could not open file '%s'.\n", fn);
        exit(1);
    }

    char ch;
    char digits[2] = {0};
    int num_for_line = 0;
    size_t sum = 0;
    size_t line_count = 0;

    // Read the file char by char
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            // Create an actual number from both digits
            sscanf(digits, "%d", &num_for_line);
            sum += num_for_line;
            // printf("Number for line: %d\n", num_for_line);
            memset(digits, 0, sizeof digits);
            num_for_line = 0;
            line_count++;
            continue;
        }

        // Keep track of digits
        if (isdigit(ch) != 0)
        {
            // Assign first digit to both array elements
            // in case there is only one digit in the line
            if (digits[0] == 0)
            {
                digits[0] = ch;
                digits[1] = ch;
            }
            else
                digits[1] = ch;
        }
    }

    printf("Processed %ld lines of text.\n", line_count);
    printf("The sum of all calibration values is %zu.\n", sum);

    fclose(fp);

    return 0;
}
