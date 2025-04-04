/*
  ==============================================================================

    Help.cpp
    Author:  aike

  ==============================================================================
*/

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>

#include "Help.h"


std::string Help::commandName(int n)
{
    if (n < 256)
    {
        return byte0[n];
    }
    return "";
}

std::string Help::commandHelp(int n, int width1, int width2)
{
    if (n < 256)
    {
        std::stringstream hex;
        hex << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << n << "h";
        return hex.str() + " " + fmt(byte0[n], width1) + " | " + fmt(byte1[n], width2) + "| " + byte2[n];
    }
    return "";
}

int Help::commandNumber(std::string s)
{
    if (cmd.find(s) != cmd.end())
    {
        return cmd[s];
    }
    return -1;
}

int Help::ccCommandNumber(std::string s)
{
    if (cc_cmd.find(s) != cc_cmd.end())
    {
        return cc_cmd[s];
    }
    return -1;
}

int Help::ccxCommandNumber(std::string s)
{
    if (ccx_cmd.find(s) != ccx_cmd.end())
    {
        return ccx_cmd[s];
    }
    return -1;
}

int Help::ccxMsbToLsb(int msb)
{
    if (msb < 0x14)
    {
        return msb + 0x20;
    }
    return msb - 1;
}

std::string Help::ccCommandName(int n)
{
    if (n < 256)
    {
        return cc1[n];
    }
    return "";
}


std::string Help::ccCommandHelp(int n, int width)
{
    if (n < 256)
    {
        std::stringstream hex;
        hex << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << n << "h";
        return hex.str() + " " + fmt(cc1[n], width) + " | " + cc2[n];
    }
    return "";
}

std::string Help::ccxCommandHelp(int n)
{
    if (n < 256)
    {
        std::stringstream hex1;
        std::stringstream hex2;
        hex1 << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << n << "h";
        hex2 << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ccxMsbToLsb(n) << "h";
        std::string s = hex1.str() + "/" + hex2.str() + " " + ccx[n] + " | " + "MSB/LSB(0-127)";

        return s;
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

std::string Help::miderCommandHelp(std::string s)
{
    if (midercmdtext.find(s) != midercmdtext.end())
    {
        return midercmdtext[s];
    }
    return "";
}


std::string Help::toString(std::vector<uint8_t> byte)
{
    std::string s = "";
    int msg = byte[0];
    MSG msgtype = getMessageType(byte);
    if ((msgtype == MSG::ChannelVoice) || (msgtype == MSG::ChannelVoiceCc) || (msgtype == MSG::ChannelMode))
    {
        msg = msg & 0xF0;
    }
    int len = getMessageLength(msg);

    if (byte.size() >= len)
    {
        if (msgtype == MSG::ChannelVoiceCc)
        {
            int cctype = byte[1];
            if (byte.size() == 6)
            {
                int val = byte[2] * 0x80 + byte[5];
                int msb = cctype;
                int lsb = ccxMsbToLsb(msb);
                s =   "[Control Change] [" + cc1[msb] + "] [" + cc2[msb] + ":" + std::to_string(byte[2]) + "]\n"
                    + "           "
                    + "[Control Change] [" + cc1[lsb] + "] [" + cc2[lsb] + ":" + std::to_string(byte[5]) + "]\n"
                    + "  value  : MSB * 80h + LSB = " + std::to_string(val);
                return s;
            }

            s = "[Control Change] [" + cc1[cctype] + "] [" + cc2[cctype] + ":" + std::to_string(byte[2]) + "]";
            return s;
        }
        else if (msgtype == MSG::ChannelMode)
        {
            int cctype = byte[1];
            s = "[Channel Mode] [" + cc1[cctype] + "] [" + cc2[cctype] + ":" + std::to_string(byte[2]) + "]";
            return s;
        }
        else if (msgtype == MSG::ChannelVoice)
        {
            msg = msg & 0xF0;
        }

        if (byte[0] == 0xF0)
        {
            // SysEx
            std::stringstream ss;
            ss << "[SOX]";
            for (int i = 1; i < byte.size(); i++)
            {
                ss << " ";
                if ((i == byte.size() - 1) && (byte[i] == 0xF7))
                {
                    ss << "[EOX]";
                }
                else
                {
                    ss << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (int)byte[i] << "h";
                }
            }
            s = ss.str();
        }
        else
        {
            switch (len) {
            case 1:
                s = "[" + byte0[msg] + "]";
                break;
            case 2:
                s = "[" + byte0[msg] + "] [" + byte1[msg] + ":" + std::to_string(byte[1]) + "]";
                break;
            case 3:
                s = "[" + byte0[msg] + "] [" + byte1[msg] + ":" + std::to_string(byte[1]) + "] [" + byte2[msg] + ":" + std::to_string(byte[2]) + "]";
                break;
            default:
                break;
            }
        }
    }
    return s;
}

MSG Help::getMessageType(std::vector<uint8_t> byte)
{
    if (byte[0] < 0x80)
    {
        return MSG::NotStatus;
    }
    else if (byte[0] < 0xF0)
    {
        if ((byte.size() > 2) && ((byte[0] & 0xF0) == 0xB0))
        {
            if (byte[1] < 0x78)
            {
                return MSG::ChannelVoiceCc; // B0 + ControlChange
            }
            else
            {
                return MSG::ChannelMode;    // B0 + NotControlChange
            }
        }
        return MSG::ChannelVoice; // 0x80-0xE0
    }
    else if (byte[0] < 0xF8)
    {
        return MSG::SystemCommon;
    }

    return MSG::SystemRealtime;
}

std::string Help::getMessageTypeName(MSG msgtype)
{
    return msgtypename[msgtype];
}

int Help::getMessageLength(int n)
{
    int ret = 1;

    if (byte1[n] == "-")
    {
        return ret;
    }
    ret++;

    if (byte2[n] == "-")
    {
        return ret;
    }
    ret++;

    return ret;
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

std::string Help::fmt(std::string s, int width)
{
    if ((int)s.size() < width)
    {
        return s + std::string(width - s.size(), ' ');
    }
    return s;
}
