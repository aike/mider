/*
  ==============================================================================

    Help.cpp
    Author:  aike

  ==============================================================================
*/

#include <algorithm>
#include <cctype>

#include "Help.h"


std::string Help::commandName(int n)
{
    if (n < 256)
    {
        return byte0[n];
    }

    return "";
}

std::string Help::ccName(int n)
{
    if (n < 256)
    {
        return byte0[n];
    }

    return "";
}

std::string Help::toString(int b0, int b1, int b2)
{
    std::string s = "";
    int command = b0 & 0xF0;
    if (commandName(command) == "Control Change")
    {
        s = "[CC] [" + cc1[b1] + "] " + cc2[b1] + ":" + std::to_string(b2);
    }
    else
    {
        s = "[" + byte0[b0] + "] [" + byte1[b0] + ":" + std::to_string(b1) + "] [" + byte2[b0] + ":" + std::to_string(b2) + "]";
    }
    return s;
}


// 大文字→小文字変換＋スペース除外
std::string Help::toLowerNoSpace(const std::string& in_s)
{
    std::string out_s = "";

    for (int i = 0; i < in_s.size(); i++) {
        if (!std::isspace(in_s[i]))
        {
            out_s += static_cast<char>(std::tolower(in_s[i]));
        }
    }

    return out_s;
}

// 先頭文字を小文字にして略語に変換
std::string Help::toAbbreviation(const std::string& in_s)
{
    std::string out_s;
    bool isStartOfWord = true;

    for (char c : in_s)
    {
        if (std::isspace(c))
        {
            isStartOfWord = true;
        }
        else if (isStartOfWord)
        {
            out_s += static_cast<char>(std::tolower(c));
            isStartOfWord = false;
        }
    }
    return out_s;
}
