/*
  ==============================================================================

    ArgParser.h
    Author:  aike

  ==============================================================================
*/

#pragma once

#include <string>
#include <vector>
#include "Help.h"

enum P {
    SHOWVERSION,
    DEVICE,
    INDEVICE,
    OUTDEVICE,
    DEV_CH_CHANNELVOICEMSG,
    DEV_CH_CC_CHANNELVOICEMSG,
    DEV_CH_CCX_CHANNELVOICEMSG,
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
    ArgParser() {
        ptext[SHOWVERSION] = "SHOWVERSION";
        ptext[DEVICE] = "DEVICE";
        ptext[INDEVICE] = "INDEVICE";
        ptext[OUTDEVICE] = "OUTDEVICE";
        ptext[DEV_CH_CHANNELVOICEMSG] = "DEV_CH_CHANNELVOICEMSG";
        ptext[DEV_CH_CC_CHANNELVOICEMSG] = "DEV_CH_CC_CHANNELVOICEMSG";
        ptext[DEV_CH_CCX_CHANNELVOICEMSG] = "DEV_CH_CCX_CHANNELVOICEMSG";
        ptext[DEV_CH_CM_CHANNELMODEMSG] = "DEV_CH_CM_CHANNELMODEMSG";
        ptext[DEV_SYSTEMCOMMONMSG] = "DEV_SYSTEMCOMMONMSG";
        ptext[DEV_SYSTEMRTMSG] = "DEV_SYSTEMRTMSG";
        ptext[DEV_BYTELIST] = "DEV_BYTELIST";
        ptext[DEV_RECEIVE] = "DEV_RECEIVE";
        ptext[NO_ARGS_HELP] = "NO_ARGS_HELP";
        ptext[HELP] = "HELP,";
        ptext[HELP_MSGNAME] = "HELP_MSGNAME";
        ptext[HELP_CC] = "HELP_CC";
        ptext[HELP_CC_CCNAME] = "HELP_CC_CCNAME";
        ptext[HELP_CM] = "HELP_CM";
        ptext[HELP_CM_CMNAME] = "HELP_CM_CMNAME";
        ptext[ERROR] = "ERROR";
        ptext[E_MIDI_DEVICE_ERROR] = "E_MIDI_DEVICE_ERROR";
        ptext[E_SYNTAX_ERROR] = "E_SYNTAX_ERROR";
        ptext[E_MSGNAME_ERROR] = "E_MSGNAME_ERROR";
        ptext[E_CCNAME_ERROR] = "E_CCNAME_ERROR";
        ptext[E_ARG_ERROR] = "E_ARG_ERROR";
        ptext[E_MSGBUF_ERROR] = "E_MSGBUF_ERROR";
    };
    ~ArgParser() {};

    P parse(std::vector<std::string>arg);
    int getDevice(void);
    int getChannel(void);
    std::vector<uint8_t>getBytes(void);
    std::string getText(void);
    std::string getMessageTypeName(P p);

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
    std::string ptext[26];

};
