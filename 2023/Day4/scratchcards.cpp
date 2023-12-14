//
// Filename:    scratchcards.cpp
// Date:        12/04/2023
// Description: Advent of Code 2023 - Day 4
// Link:        https://adventofcode.com/2023/day/4
// Author:      Jon Holland
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string_view>

// Forward function declarations
std::vector<std::string> split_string(const std::string& str, char delimiter);
std::vector<std::string_view> split_at_any(const std::string_view input,
                                           const std::string_view delimiters);

int main()
{
    using namespace std;

    ifstream fin;
    string line;
    int total_points = 0;

    fin.open("input");

    while (getline(fin, line))
    {
        vector<string> winning_nums;
        vector<string> my_nums;
        int card_points;
        
        for (const auto ss : split_at_any(line, ":|"))
        {
            bool is_card_num = ss.find("Card") != string::npos;
            bool is_winning_nums = ss.length() < 32;
            
            // Skip the card number field
            if (is_card_num)
            {
                card_points = 0;
                continue;
            }
            
            // The winning numbers field is shorter
            if (is_winning_nums)
                winning_nums = split_string((string)ss, ' ');
            else
            {
                vector<string>::iterator i;
                my_nums = split_string((string)ss, ' ');

                // Calculate points for this card
                for (i = my_nums.begin(); i != my_nums.end(); ++i)
                {
                    if (find(winning_nums.begin(),
                             winning_nums.end(), *i) != winning_nums.end())
                    {
                        if (card_points == 0)
                            card_points = 1;
                        else
                            card_points *= 2;

                        // cout << "Card points: " << card_points << endl;
                    }
                }

                // Update total points
                total_points += card_points;
            }
        }
    }

    fin.close();
    cout << "The scratchcards are worth " << total_points << " points in total." << endl;

    return 0;
}

std::vector<std::string> split_string(const std::string& str, char delimiter)
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
    for (std::size_t i = 0; i < input.size(); ++i)
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
