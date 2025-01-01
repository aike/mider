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
    INDEVICE,
    OUTDEVICE,
    DEV_CH_CHANNELVOICEMSG,
    DEV_CH_CC_CHANNELVOICEMSG,
    DEV_CH_CM_CHANNELMODEMSG,
    DEV_SYSTEMCOMMONMSG,
    DEV_SYSTEMRTMSG,
    DEV_BYTELIST,
    DEV_RECEIVE,

    NO_ARGS_HELP,
    HELP,
    HELP_MSGNAME,
    HELP_CC,
    HELP_CC_CCNAME,
    HELP_CM,
    HELP_CM_CMNAME,

    ERROR,
    E_MIDI_DEVICE_ERROR,
    E_SYNTAX_ERROR,
    E_MSGNAME_ERROR,
    E_CCNAME_ERROR,
    E_ARG_ERROR,
    E_MSGBUF_ERROR
};


class ArgParser
{
public:
    ArgParser() {};
    ~ArgParser() {};

    P parse(std::vector<std::string>arg);
    int getDevice(void);
    int getChannel(void);
    std::vector<uint8_t>getBytes(void);
    std::string getText(void);

private:

    std::string toLower(std::string s);
    bool isAlphabet(std::string s);
    bool isInt1to16(std::string s);
    bool isInt128(std::string s);
    bool isInt256(std::string s);
    int getNumber(std::string s, int defval = -1);
    void setBytes(std::vector<int> byteVec) {
        for (int i = 0; i < byteVec.size(); i++)
        {
            bytes.push_back((uint8_t)byteVec[i]);
        }
    };

    Help h;
    int device = 0;
    int channel = 0;
    std::vector<uint8_t> bytes;
    std::string text;
};
