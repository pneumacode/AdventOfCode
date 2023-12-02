//
// Filename:    trebuchet2.c
// Date:        12/01/2023
// Description: Advent of Code 2023 - Day 1, Part 2
// Link:        https://adventofcode.com/2023/day/1
// Author:      Jon Holland
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = {0};
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1)
    {
        const char *p = strstr(tmp, needle);

        // Past last occurrence of needle; copy remaining part
        if (p == NULL)
        {
            strcpy(insert_point, tmp);
            break;
        }

        // Copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // Copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // Adjust pointers, move on
        tmp = p + needle_len;
    }

    // Write modified string back to target
    strcpy(target, buffer);
}

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

    char line[128];
    char digits[2] = {0};
    int num_for_line = 0;
    size_t sum = 0;
    size_t line_count = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // Normalize textual numbers
        // TODO: A better approach is needed to handle overlapping cases.
        str_replace(line, "eightwo", "82");
        str_replace(line, "twone", "21");
        str_replace(line, "oneight", "18");
        str_replace(line, "one", "1");
        str_replace(line, "two", "2");
        str_replace(line, "three", "3");
        str_replace(line, "four", "4");
        str_replace(line, "five", "5");
        str_replace(line, "six", "6");
        str_replace(line, "seven", "7");
        str_replace(line, "eight", "8");
        str_replace(line, "nine", "9");
        // printf("line = %s", line);

        // Read the line char by char
        for (int i = 0; i < (int)strlen(line); i++)
        {
            // Keep track of digits
            if (isdigit(line[i]) != 0)
            {
                // Assign first digit to both array elements
                // in case there is only one digit in the line
                if (digits[0] == 0)
                {
                    digits[0] = line[i];
                    digits[1] = line[i];
                }
                else
                    digits[1] = line[i];
            }
        }

        // Create an actual number from both digits
        sscanf(digits, "%2d", &num_for_line);
        sum += num_for_line;
        // printf("Number for line: %d\n", num_for_line);
        memset(digits, 0, sizeof digits);
        num_for_line = 0;
        line_count++;
    }

    printf("Processed %ld lines of text.\n", line_count);
    printf("The sum of all calibration values is %zu.\n", sum);

    fclose(fp);

    return 0;
}
