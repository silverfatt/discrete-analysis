#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>

std::pair<int, int> GetStringNumber(int number, const std::vector<int> &string_lengths)
{
    int string_number = 0;
    while (number - string_lengths[string_number] > 0)
    {
        number -= string_lengths[string_number];
        ++string_number;
    }
    ++string_number;
    return {string_number, number};
}

std::vector<int> NaiveSearch(std::vector<std::string> text, std::vector<std::string> pattern)
{
    std::vector<int> positions;
    for (int i = 0; i < text.size() - pattern.size(); ++i)
    {
        int j = i;
        for (int k = 0; k < pattern.size(); ++k)
        {
            if (pattern[k] != text[j])
            {
                break;
            }
            else if (pattern[k] == text[j] && k + 1 == pattern.size())
            {
                positions.push_back(i);
            }
            else
            {
                ++j;
            }
        }
    }
    return positions;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int start_time = clock();
    std::string string, pstring;
    std::vector<std::string> pattern, text;
    std::vector<int> string_lengths;
    std::getline(std::cin, pstring);
    std::stringstream pattern_stream(pstring);
    while (pattern_stream >> pstring)
    {
        std::transform(pstring.begin(), pstring.end(),
                       pstring.begin(), tolower);
        pattern.push_back(pstring);
    }
    while (std::getline(std::cin, string))
    {
        std::stringstream text_stream(string);
        int string_len = 0;
        while (text_stream >> string)
        {
            ++string_len;
            std::transform(string.begin(), string.end(),
                           string.begin(), tolower);
            text.push_back(string);
        }
        string_lengths.push_back(string_len);
    }

    std::vector<int> res = NaiveSearch(text, pattern);
    for (auto &r : res)
    {
        auto p = GetStringNumber(r + 1, string_lengths);
        std::cout << p.first << ", " << p.second << "\n";
    }
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    std::cout << search_time;
    return 0;
}