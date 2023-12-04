//
// Filename:    cubes.cpp
// Date:        12/02/2023
// Description: Advent of Code 2023 - Day 2
// Link:        https://adventofcode.com/2023/day/2
// Author:      Jon Holland
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string_view>

std::vector<std::string> splitString(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos)
    {
        std::string token = str.substr(start, end - start);
        if (!token.empty())
        {
            tokens.push_back(token);
        }

        start = end + 1;
        end = str.find(delimiter, start);
    }

    std::string lastToken = str.substr(start);
    if (!lastToken.empty())
    {
        tokens.push_back(lastToken);
    }

    return tokens;
}

std::vector<std::string_view> split_at_any(const std::string_view input,
                                           const std::string_view delimiters)
{
    std::vector<std::string_view> result;
    std::size_t last_pos = 0;
    for (std::size_t i=0; i < input.size(); ++i)
    {
        if (delimiters.find(input[i]) != delimiters.npos)
        {
            result.push_back(input.substr(last_pos, i - last_pos));
            ++i;
            last_pos = i;
        }
    }
    
    // Add the final line item to the array because it was
    // unclear how to add a newline to the list of delimiters.
    result.push_back(input.substr(last_pos, input.size()));

    return result;
}

int main()
{
	const int MAX_RED   = 12;
	const int MAX_GREEN = 13;
	const int MAX_BLUE  = 14;

    using namespace std;

    ifstream fin;
    string line;
	int id_sums = 0;
	int game_counter = 0;
    bool rPossible = true, gPossible = true, bPossible = true;

    fin.open("input");

    while (getline(fin, line))
    {
        for (const auto ss : split_at_any(line, ",;:"))
        {
			bool isGame  = ss.find("Game")  != string::npos;
			bool isRed   = ss.find("red")   != string::npos;
			bool isGreen = ss.find("green") != string::npos;
			bool isBlue  = ss.find("blue")  != string::npos;

			if (isGame)
			{
                if (game_counter > 0 && rPossible && gPossible && bPossible)
                    id_sums += game_counter;

                // Assume the next game is possible for now.
                rPossible = true, gPossible = true, bPossible = true;
				game_counter++;
			}

			if (isRed)
			{
				vector<string> red = splitString((string)ss, ' ');
                // Is this game possible?
                if (stoi(red[0]) > MAX_RED)
                {
                    rPossible = false;
                }
			}

			if (isGreen)
			{
                vector<string> green = splitString((string)ss, ' ');
                // Is this game possible?
                if (stoi(green[0]) > MAX_GREEN)
                {
                    gPossible = false;
                }
			}

			if (isBlue)
			{
                vector<string> blue = splitString((string)ss, ' ');
                // Is this game possible?
                if (stoi(blue[0]) > MAX_BLUE)
                {
                    bPossible = false;
                }
			}
        }
    }

    // Check the final game result.
    if (rPossible && gPossible && bPossible)
        id_sums += game_counter;

    fin.close();
    printf("The sum of all possible Game IDs is %d.\n", id_sums);

    return 0;
}
