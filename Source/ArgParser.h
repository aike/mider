/*
  ==============================================================================

    ArgParser.h
    Created: 28 Dec 2024 3:36:49pm
    Author:  ai

  ==============================================================================
*/

#pragma once

#include <string>
#include <vector>
#include "Help.h"

enum P {
    DEVICE,
    DEV_CH_MSGNAME,
    DEV_CH_CC_CCNAME,
    DEV_CH_BYTE0_BYTE1_BYTE2,

    NO_ARGS_HELP,
    HELP,
    HELP_MSGNAME,
    HELP_CC,
    HELP_CC_CCNAME,

    E_SYNTAX_ERROR,
    E_MSGNAME_ERROR,
    E_CCNAME_ERROR,
    E_ARG_ERROR
};


class ArgParser
{
public:
    ArgParser() {};
    ~ArgParser() {};

    P parse(std::vector<std::string>arg);
    int getDevice(void);
    int getChannel(void);
    std::vector<int>getBytes(void);
    std::string getMessage(void);

private:

    std::string toLower(std::string s);
    bool isAlphabet(std::string s);
    bool isInt1to16(std::string s);
    bool isInt128(std::string s);
    bool isInt256(std::string s);
    int getNumber(std::string s, int defval = -1);

    Help h;
    std::vector<int> bytes;
    std::string message;
};
