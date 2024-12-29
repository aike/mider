/*
  ==============================================================================

    ArgParser.cpp
    Created: 28 Dec 2024 3:36:49pm
    Author:  ai

  ==============================================================================
*/


#include <regex>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "ArgParser.h"

P ArgParser::parse(std::vector<std::string>arg)
{
    // clear parse result
    bytes.clear();
    message = "";

    // convert from char* array to std::string array
    while (arg.size() < 10)
    {
        arg.push_back("");
    }

    ////// (0) mider ////////
    if (arg[1] == "")
    {
        return P::NO_ARGS_HELP;
    }

    ////// (1) mider devices ////////
    else if ((arg[1] == "devices") && (arg[2] == ""))
    {
        return P::DEVICE;
    }

    ////// (2) mider device channel command byte1 byte2 ////////
    else if (isInt128(arg[1]) && isInt1to16(arg[2]) && isAlphabet(arg[3]) && !isAlphabet(arg[4]))
    {
        device = std::stoi(arg[1]);
        channel = std::stoi(arg[2]) - 1;
        int byte0 = h.commandNumber(toLower(arg[3]));
        if (byte0 < 0)
        {
            message = "ERROR: Massage name(" + arg[3] + ") not found.";
            return P::E_MSGNAME_ERROR;
        }

        int len = h.getMessageLength(byte0);
        if (len == 1)
        {
            setBytes({ byte0 + channel });
        }
        else if (len == 2)
        {
            int byte1 = getNumber(arg[4]);
            if (byte1 < 0)
            {
                message = "ERROR: Argument syntax error.\n";
                message += "  " + h.commandHelp1(byte0);
                return P::E_SYNTAX_ERROR;
            }
            setBytes({ byte0 + channel, byte1 });
        }
        else if (len == 3)
        {
            int byte1 = getNumber(arg[4]);
            int byte2 = getNumber(arg[5]);
            if ((byte1 < 0) || (byte2 < 0))
            {
                message = "ERROR: Argument syntax error.\n";
                message += "  " + h.commandHelp1(byte0);
                return P::E_SYNTAX_ERROR;
            }
            setBytes({ byte0 + channel, byte1, byte2 });
        }

        return P::DEV_CH_MSGNAME;
    }

    ////// (3) mider device channel "cc" cc_command byte2 ////////
    else if (isInt128(arg[1]) && isInt1to16(arg[2]) && isAlphabet(arg[3]) && isAlphabet(arg[4]))
    {
        device = std::stoi(arg[1]);
        channel = std::stoi(arg[2]) - 1;
        int byte0 = h.commandNumber(toLower(arg[3]));
        int byte1 = h.ccCommandNumber(toLower(arg[4]));
        int byte2 = getNumber(arg[5], 0);

        if (byte0 != 0xB0)
        {
            message = "ERROR: Argument syntax error.";
            return P::E_SYNTAX_ERROR;
        }
        if (byte1 < 0)
        {
            message = "ERROR: CC name(" + arg[4] + ") not found.";
            return P::E_CCNAME_ERROR;
        }

        setBytes({ byte0 + channel, byte1, byte2 });

        return P::DEV_CH_CC_CCNAME;
    }

    ////// (4) mider device byte0 byte1 byte2 ////////
    else if (isInt128(arg[1]) && isInt256(arg[2]))
    {
        device = std::stoi(arg[1]);
        channel = getNumber(arg[2]) & 0x0F;

        std::vector<int> bytelist;
        for (int i = 2; i < arg.size(); i++)
        {
            int n = getNumber(arg[i], -1);
            if (n < 0) {
                break;
            }
            bytelist.push_back(n);
        }
        setBytes(bytelist);

        return P::DEV_BYTELIST;
    }

    ////// (5) mider help ////////
    else if (arg[1] == "help")
    {
        if (arg[2] == "")
        {
            return P::HELP;
        }
        else if (isAlphabet(arg[2]) && arg[3] == "")
        {
            int n = h.commandNumber(toLower(arg[2]));
            if (n < 0)
            {
                message = arg[2] + " is not a message.";
                return P::E_MSGNAME_ERROR;
            }
            else if (h.commandName(n) == "Control Change")
            {
                return P::HELP_CC;
            }
            return P::HELP_MSGNAME;
        }
        else if (isAlphabet(arg[2]) && isAlphabet(arg[3]))
        {
            int n = h.commandNumber(toLower(arg[2]));
            if (h.commandName(n) == "Control Change")
            {
                int m = h.ccCommandNumber(toLower(arg[3]));
                if (m < 0)
                {
                    message = arg[3] + " is not a control change.";
                    return P::E_CCNAME_ERROR;
                }
                else if ((m < 120))
                {
                    message = "Control Change Help:\n";
                    message += "  B0h Control Change | " + h.ccCommandHelp1(m);
                    return P::HELP_CC_CCNAME;
                }
                else
                {
                    message = "Channel Mode Message Help:\n";
                    message += "  B0h Channel Mode | " + h.ccCommandHelp1(m);
                    return P::HELP_CM_CMNAME;
                }
            }

            // TODO:エラーメッセージ見直し
            message = arg[2] + " is not a message.";
            return P::E_SYNTAX_ERROR;
        }

        // ここにはこないはず
        return P::E_ARG_ERROR;
    }

    ////// the other argument pattern /////////
    message = "ERROR: Argument syntax error.";
    return P::E_ARG_ERROR;
}

int ArgParser::getDevice(void)
{
    return device;
}

int ArgParser::getChannel(void)
{
    return channel;
}

std::string ArgParser::getMessage(void)
{
    return message;
}

std::vector<uint8_t> ArgParser::getBytes(void)
{
    return bytes;
}

std::string ArgParser::toLower(std::string s)
{
    std::string s2 = s;

    for (int i = 0; i < s.size(); i++) {
        s2[i] = static_cast<char>(std::tolower(s2[i]));
    }

    return s2;
}


bool ArgParser::isAlphabet(std::string s)
{
    std::regex re_command(R"(^[a-zA-Z][a-zA-Z0-9]*$)");
    std::smatch match;

    if (std::regex_match(s, match, re_command))
    {
        return true;
    }
    return false;
}

bool ArgParser::isInt1to16(std::string s)
{
    int n = getNumber(s);
    if ((n < 1) || (n > 16))
    {
        return false;
    }
    return true;
}

bool ArgParser::isInt128(std::string s)
{
    int n = getNumber(s);
    if ((n < 0) || (n > 127))
    {
        return false;
    }
    return true;
}

bool ArgParser::isInt256(std::string s)
{
    int n = getNumber(s);
    if ((n < 0) || (n > 255))
    {
        return false;
    }
    return true;
}

int ArgParser::getNumber(std::string s, int defval)
{
    std::regex re_dec(R"(^\d?\d?\d$)");
    std::regex re_hex1(R"(^[0-9a-fA-F]?[0-9a-fA-F]h$)");
    std::regex re_hex2(R"(^0x[0-9a-fA-F]?[0-9a-fA-F]$)");
    std::smatch match;
    int value = defval;

    if (std::regex_match(s, match, re_dec))
    {
        value = std::stoi(match[0].str());
    }
    else if (std::regex_match(s, match, re_hex1))
    {
        s.pop_back();
        value = std::stoi(match[0].str(), nullptr, 16);
    }
    else if (std::regex_match(s, match, re_hex2))
    {
        s.erase(0, 2);
        value = std::stoi(match[0].str(), nullptr, 16);
    }

    return value;
}

