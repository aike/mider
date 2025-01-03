/*
  ==============================================================================

    ArgParser.cpp
    Author:  aike

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
    text = "";

    // convert from char* array to std::string array
    while (arg.size() < 10)
    {
        arg.push_back("");
    }

    std::string arg1Lower = toLower(arg[1]);

    ////// mider ////////
    if (arg[1] == "")
    {
        return P::NO_ARGS_HELP;
    }

    ////// mider command ////////
    ///  ex. mider devices
    else if ((arg1Lower == "version") && (arg[2] == ""))
    {
        return P::SHOWVERSION;
    }
    else if ((arg1Lower == "--version") && (arg[2] == ""))
    {
        return P::SHOWVERSION;
    }
    else if ((arg1Lower == "devices") && (arg[2] == ""))
    {
        return P::DEVICE;
    }
    else if ((arg1Lower == "indevices") && (arg[2] == ""))
    {
        return P::INDEVICE;
    }
    else if ((arg1Lower == "outdevices") && (arg[2] == ""))
    {
        return P::OUTDEVICE;
    }

    ////// mider device receive ////////
    ///  ex. mider 1 receive
    else if (isInt128(arg[1]) && (arg[2] == "receive") && (arg[3] == ""))
    {
        device = std::stoi(arg[1]);
        return P::DEV_RECEIVE;
    }

    ////// mider device channel command [byte1 [byte2]] ////////
    ///  ex. mider 1 1 noteon 60 100
    else if (isInt128(arg[1]) && isInt1to16(arg[2]) && isAlphabet(arg[3]) && !isAlphabet(arg[4]))
    {
        device = std::stoi(arg[1]);
        channel = std::stoi(arg[2]) - 1;
        int byte0 = h.commandNumber(toLower(arg[3]));
        if (byte0 < 0)
        {
            // mider device channel cccommand byte1
            int byte1 = h.ccCommandNumber(toLower(arg[3]));
            if (byte1 >= 0)
            {
                int byte2 = getNumber(arg[4]);
                if (byte2 < 0)
                {
                    text = "ERROR: Argument syntax error.\n";
                    text += "  " + h.ccCommandHelp1(byte1);
                    return P::E_SYNTAX_ERROR;
                }
                setBytes({ 0xB0 + channel, byte1, byte2 });
                if (h.getMessageType(getBytes()) == MSG::ChannelVoiceCc)
                {
                    return P::DEV_CH_CC_CHANNELVOICEMSG;
                }
                else
                {
                    return P::DEV_CH_CM_CHANNELMODEMSG;
                }
            }
            else if ((h.ccxCommandNumber(toLower(arg[3])) >= 0) && (getNumber(arg[4]) > 0) && (getNumber(arg[5]) > 0))
            {
                byte1 = h.ccxCommandNumber(toLower(arg[3]));
                int byte2 = getNumber(arg[4]);
                int byte3 = getNumber(arg[5]);
                setBytes({ 0xB0 + channel, byte1, byte2, 0xB0 + channel, h.ccxMsbToLsb(byte1), byte3 });
                return P::DEV_CH_CCX_CHANNELVOICEMSG;
            }

            text = "ERROR: Massage name(" + arg[3] + ") not found.";
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
                text = "ERROR: Argument syntax error.\n";
                text += "  " + h.commandHelp1(byte0);
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
                text = "ERROR: Argument syntax error.\n";
                text += "  " + h.commandHelp1(byte0);
                return P::E_SYNTAX_ERROR;
            }
            setBytes({ byte0 + channel, byte1, byte2 });
        }

        return P::DEV_CH_CHANNELVOICEMSG;
    }

    ////// mider device command [byte1 [byte2]] ////////
    ///  ex. mider 1 TuneRequest
    else if (isInt128(arg[1]) && isAlphabet(arg[2]) && !isInt256(arg[2]) && !isAlphabet(arg[3]))
    {
        device = std::stoi(arg[1]);
        channel = -1;
        int byte0 = h.commandNumber(toLower(arg[2]));
        if (byte0 < 0)
        {
            text = "ERROR: Massage name(" + arg[2] + ") not found.";
            return P::E_MSGNAME_ERROR;
        }

        int len = h.getMessageLength(byte0);
        if (byte0 == 0xF0)
        {
            // SysEx
            std::vector<int> sysexBytes;
            sysexBytes.reserve(arg.size() - 2);
            sysexBytes.push_back(0xF0);
            for (int i = 3; i < arg.size(); i++)
            {
                int b = getNumber(arg[i]);

                if (b < 0)
                {
                    if (toLower(arg[i]) == "eox")
                    {
                        b = 0xF7;
                    }
                    else
                    {
                        break;
                    }
                }
                sysexBytes.push_back(b);
            }
            setBytes(sysexBytes);
        }
        else if (len == 1)
        {
            setBytes({ byte0 });
        }
        else if (len == 2)
        {
            int byte1 = getNumber(arg[3]);
            if (byte1 < 0)
            {
                text = "ERROR: Argument syntax error.\n";
                text += "  " + h.commandHelp1(byte0);
                return P::E_SYNTAX_ERROR;
            }
            setBytes({ byte0, byte1 });
        }
        else if (len == 3)
        {
            int byte1 = getNumber(arg[3]);
            int byte2 = getNumber(arg[4]);
            if ((byte1 < 0) || (byte2 < 0))
            {
                text = "ERROR: Argument syntax error.\n";
                text += "  " + h.commandHelp1(byte0);
                return P::E_SYNTAX_ERROR;
            }
            setBytes({ byte0, byte1, byte2 });
        }

        if (h.getMessageType(getBytes()) == MSG::SystemCommon)
        {
            return P::DEV_SYSTEMCOMMONMSG;
        }

        return P::DEV_SYSTEMRTMSG;
    }

    ////// mider device channel "cc" cc_command byte2 ////////
    ///  ex. mider 1 1 CC BankSelect 1
    else if (isInt128(arg[1]) && isInt1to16(arg[2]) && isAlphabet(arg[3]) && isAlphabet(arg[4]))
    {
        device = std::stoi(arg[1]);
        channel = std::stoi(arg[2]) - 1;
        int byte0 = h.commandNumber(toLower(arg[3]));
        int byte1 = h.ccCommandNumber(toLower(arg[4]));
        int byte2 = getNumber(arg[5]);

        if (byte0 != 0xB0)
        {
            text = "ERROR: Argument syntax error.";
            return P::E_SYNTAX_ERROR;
        }
        if (byte1 < 0)
        {
            if ((h.ccxCommandNumber(toLower(arg[4])) >= 0) && (getNumber(arg[5]) > 0) && (getNumber(arg[6]) > 0))
            {
                byte1 = h.ccxCommandNumber(toLower(arg[4]));
                byte2 = getNumber(arg[5]);
                int byte3 = getNumber(arg[6]);
                setBytes({ 0xB0 + channel, byte1, byte2, 0xB0 + channel, h.ccxMsbToLsb(byte1), byte3 });
                return P::DEV_CH_CCX_CHANNELVOICEMSG;
            }
            text = "ERROR: CC name(" + arg[4] + ") not found.";
            return P::E_CCNAME_ERROR;
        }
        if (byte2 < 0)
        {
            text = "ERROR: Argument syntax error.";
            return P::E_SYNTAX_ERROR;
        }

        setBytes({ byte0 + channel, byte1, byte2 });
        if (h.getMessageType(getBytes()) == MSG::ChannelVoiceCc)
        {
            return P::DEV_CH_CC_CHANNELVOICEMSG;
        }
        else
        {
            return P::DEV_CH_CM_CHANNELMODEMSG;
        }

        return P::DEV_CH_CC_CHANNELVOICEMSG;
    }

    ////// mider device byte [byte1 ...] ////////
    ///  ex. mider 1 90h 3Ch 1Eh
    else if (isInt128(arg[1]) && isInt256(arg[2]))
    {
        device = std::stoi(arg[1]);

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

    ////// mider help [command ...] ////////
    ///  ex. mider help
    else if (arg[1] == "help")
    {
        if (arg[2] == "")
        {
            return P::HELP;
        }
        else if (isAlphabet(arg[2]) && arg[3] == "")
        {
            std::string cmd = toLower(arg[2]);
            int n = h.commandNumber(cmd);
            if (n < 0)
            {
                int m = h.ccCommandNumber(cmd);
                if (m >= 0)
                {
                    if (m < 0x78)
                    {
                        text = "Control Change Help:\n";
                        text += "  B0h Control Change | " + h.ccCommandHelp1(m);
                        return P::HELP_CC_CCNAME;
                    }
                    else
                    {
                        text = "Channel Mode Message Help:\n";
                        text += "  B0h Channel Mode | " + h.ccCommandHelp1(m);
                        return P::HELP_CM_CMNAME;
                    }
                }
                else if (h.ccxCommandNumber(cmd) >= 0)
                {
                    m = h.ccxCommandNumber(cmd);
                    text = "Control Change Help:\n";
                    text += "  B0h Control Change | " + h.ccCommandHelp1(m);
                    return P::HELP_CC_CCNAME;
                }
                else if (h.miderCommandHelp(cmd) != "")
                {
                    text = "Mider Command Help:\n";
                    text += "  " + h.miderCommandHelp(cmd);
                    return P::HELP_MIDERCOMMAND;
                }

                text = arg[2] + " is not a valid keyword.";
                return P::E_MSGNAME_ERROR;
            }
            else if ((cmd == "cc") || (cmd == "controlchange"))
            {
                return P::HELP_CC;
            }
            else if ((cmd == "cm") || (cmd == "channelmode"))
            {
                return P::HELP_CM;
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
                    m = h.ccxCommandNumber(toLower(arg[3]));
                    if (m >= 0)
                    {
                        text = "Control Change Help:\n";
                        text += "  B0h Control Change | " + h.ccCommandHelp1(m);
                        return P::HELP_CC_CCNAME;
                    }
                    text = arg[3] + " is not a control change.";
                    return P::E_CCNAME_ERROR;
                }
                else if ((m < 0x78))
                {
                    text = "Control Change Help:\n";
                    text += "  B0h Control Change | " + h.ccCommandHelp1(m);
                    return P::HELP_CC_CCNAME;
                }
                else
                {
                    text = "Channel Mode Message Help:\n";
                    text += "  B0h Channel Mode | " + h.ccCommandHelp1(m);
                    return P::HELP_CM_CMNAME;
                }
            }

            // TODO:エラーメッセージ見直し
            text = arg[2] + " is not a message.";
            return P::E_SYNTAX_ERROR;
        }

        // ここにはこないはず
        return P::E_ARG_ERROR;
    }

    ////// the other argument pattern /////////
    text = "ERROR: Argument syntax error.";
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

std::string ArgParser::getText(void)
{
    return text;
}

std::string ArgParser::getMessageTypeName(P p)
{
    return ptext[p];
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
    std::regex re_hex1(R"(^[0-9a-fA-F]?[0-9a-fA-F][hH]$)");
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

