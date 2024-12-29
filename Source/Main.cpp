/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include <regex>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>


#include "Help.h"
#include "ArgParser.h"

void usage(void);
void listDevices(void);
//std::string toLower(std::string s);
void sendMessage(int device, int channel, int byte0, int byte1, int byte2);

//bool isAlphabet(std::string s);
//bool isInt1to16(std::string s);
//bool isInt128(std::string s);
//bool isInt256(std::string s);
//int getNumber(std::string s, int defval = -1);

Help h;
ArgParser parser;

#define TEST

#include "Test.h"


//==============================================================================
#ifndef TEST
int main (int argc, char* argv[])
{
    std::vector<int>b;

    std::vector<std::string> args(argv, argv + argc);

    switch (parser.parse(args))
    {
    case P::DEVICE:
        listDevices();
        break;
    case P::DEV_CH_MSGNAME:
    case P::DEV_CH_CC_CCNAME:
    case P::DEV_CH_BYTE0_BYTE1_BYTE2:
        b = parser.getBytes();
        sendMessage(parser.getDevice(), parser.getChannel(), b[0], b[1], b[2]);
        break;
 
    case P::NO_ARGS_HELP:
        usage();
        break;
    case P::HELP:
        break;
    case P::HELP_MSGNAME:
        break;
    case P::HELP_CC_CCNAME:
        break;

    case P::E_SYNTAX_ERROR:
        break;
    case P::E_MSGNAME_ERROR:
        break;
    case P::E_CCNAME_ERROR:
        break;
    case P::E_ARG_ERROR:
        break;
    default:
        break;
    }

    // convert from char* array to std::string array

    //std::string arg[10];
    //if (argc > 10)
    //{
    //    argc = 10;
    //}
    //for (int i = 0; i < argc; i++)
    //{
    //    arg[i] = argv[i];
    //}
    //for (int i = argc; i < 10; i++)
    //{
    //    arg[i] = "";
    //}


    ////// (0) mider ////////
    //if (arg[1] == "")
    //{
    //    usage();
    //    return 1;
    //}

    ////// (1) mider devices ////////
    //else if ((arg[1] == "devices") && (arg[2] == ""))
    //{
    //    listDevices();
    //    return 0;
    //}

    ////// (2) mider device channel command byte1 byte2 ////////
    //else if (isInt128(arg[1]) && isInt1to16(arg[2]) && isAlphabet(arg[3]) && !isAlphabet(arg[4]))
    //{
    //    int device = std::stoi(arg[1]);
    //    int ch0    = std::stoi(arg[2]) - 1;
    //    int byte0  = h.commandNumber(toLower(arg[3]));
    //    int byte1  = getNumber(arg[4], 0);
    //    int byte2  = getNumber(arg[5], 0);

    //    if (byte0 < 0)
    //    {
    //        std::cerr << "ERROR: Massage name(" << arg[3] << ") not found." << std::endl;
    //        exit(1);
    //    }

    //    sendMessage(device, ch0, byte0 + ch0, byte1, byte2);
    //}

    ////// (3) mider device channel "cc" cc_command byte2 ////////
    //else if (isInt128(arg[1]) && isInt1to16(arg[2]) && isAlphabet(arg[3]) && isAlphabet(arg[4]))
    //{
    //    int device = std::stoi(arg[1]);
    //    int ch0 = std::stoi(arg[2]) - 1;
    //    int byte0 = h.commandNumber(toLower(arg[3]));
    //    int byte1 = h.ccCommandNumber(toLower(arg[4]));
    //    int byte2 = getNumber(arg[5], 0);

    //    if (byte0 != 0xB0)
    //    {
    //        std::cerr << "ERROR: Argument syntax error." << std::endl;
    //        exit(1);
    //    }
    //    if (byte1 < 0)
    //    {
    //        std::cerr << "ERROR: CC name(" << arg[4] << ") not found." << std::endl;
    //        exit(1);
    //    }

    //    sendMessage(device, ch0, byte0 + ch0, byte1, byte2);
    //}

    ////// (4) mider device byte0 byte1 byte2 ////////
    //else if (isInt128(arg[1]) && isInt256(arg[2]) && isInt256(arg[3]) && isInt256(arg[4]))
    //{
    //    int device = std::stoi(arg[1]);
    //    int ch0    = getNumber(arg[2]) & 0x0F;
    //    int byte0  = getNumber(arg[2]);
    //    int byte1  = getNumber(arg[3]);
    //    int byte2  = getNumber(arg[4]);

    //    sendMessage(device, ch0, byte0 + ch0, byte1, byte2);
    //}

    ////// (5) mider help ////////
//    else if (arg[1] == "help")
//    {
//        if (arg[2] == "")
//        {
//            std::cout << std::endl;
//            std::cout << "Message Help:" << std::endl;
//            for (int i = 0; i < 256; i++)
//            {
//                if (h.commandName(i) != "Undefined")
//                {
//                    std::cout << "  " << h.commandHelp(i) << std::endl;
//                }
//            }
//            std::cout << std::endl;
//        }
//        else if (isAlphabet(arg[2]) && arg[3] == "")
//        {
//            int n = h.commandNumber(arg[2]);
//            if (n < 0)
//            {
//                std::cout << arg[2] << " is not a message." << std::endl;
//                exit(1);
//            }
//
//            std::cout << std::endl;
//            std::cout << "Message Help:" << std::endl;
//            std::cout << "  " << h.commandHelp1(n) << std::endl;
//            if (h.commandName(n) == "Control Change")
//            {
//                std::cout << std::endl;
//                std::cout << "Control Change Help:" << std::endl;
//                for (int i = 0; i < 256; i++)
//                {
//                    if (h.ccCommandName(i) != "Undefined")
//                    {
//                        std::cout << "  " << h.ccCommandHelp(i) << std::endl;
//                    }
//                }
//            }
//        }
//        else if (isAlphabet(arg[2]) && isAlphabet(arg[3]))
//        {
//            int n = h.commandNumber(arg[2]);
//            if (h.commandName(n) != "Control Change")
//            {
//                std::cout << arg[2] << " is not a message." << std::endl;
//                exit(1);
//            }
//
//            int m = h.ccCommandNumber(arg[3]);
//            if (m < 0)
//            {
//                std::cout << arg[3] << " is not a control change." << std::endl;
//                exit(1);
//            }
//
////            std::cout << std::endl;
////            std::cout << std::endl;
////            std::cout << "Message Help:" << std::endl;
////            std::cout << "  " << h.commandHelp1(n) << std::endl;
//            std::cout << std::endl;
//            std::cout << "Control Change Help:" << std::endl;
//            std::cout << "  B0h Control Change | " << h.ccCommandHelp1(m) << std::endl;
//        }
//        std::cout << std::endl;
//
//        return 0;
//    }
//
//    ////// the other argument pattern /////////
//    else
//    {
//        std::cerr << "ERROR: Argument syntax error." << std::endl;
//        exit(1);
//    }

    return 0;
}
#endif


void usage(void)
{
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "    (1) mider \"devices\"" << std::endl;
    std::cout << "    (2) mider device_number channel_number message_name [byte1] [byte2]" << std::endl;
    std::cout << "    (3) mider device_number channel_number \"CC\" cc_name [byte2]" << std::endl;
    std::cout << "    (4) mider device_number byte0 byte1 byte2" << std::endl;
    std::cout << "    (5) mider \"help\"" << std::endl;
    std::cout << "        mider \"help\" message_name" << std::endl;
    std::cout << "        mider \"help\" \"CC\"" << std::endl;
    std::cout << "        mider \"help\" \"CC\" cc_name" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "    mider devices" << std::endl;
    std::cout << "    mider 1 1 NoteOn 60 100" << std::endl;
    std::cout << "    mider 1 1 ControlChange DamperPedal 0" << std::endl;
    std::cout << "    mider 1 1 CC AllNotesOff" << std::endl;
    std::cout << "    mider 1 1 cc ano" << std::endl;
    std::cout << "    mider 1 144 60 100" << std::endl;
    std::cout << "    mider 1 90h 3Ch 64h" << std::endl;
    std::cout << "    mider 1 0x90 0x3C 0x64" << std::endl;
    std::cout << std::endl;
    std::cout << "    mider help" << std::endl;
    std::cout << "    mider help NoteOn" << std::endl;
    std::cout << "    mider help CC" << std::endl;
    std::cout << "    mider help CC AllNotesOff" << std::endl;
    std::cout << std::endl;
}

void listDevices(void)
{
    // List Output Devices
    auto midiOutputDevices = juce::MidiOutput::getAvailableDevices();
    std::cout << std::endl;
    std::cout << "MIDI Devices:" << std::endl;
    for (int i = 0; i < midiOutputDevices.size(); i++)
    {
        const auto& device = midiOutputDevices[i];
        std::cout << "  " << (i + 1) << " " << device.name.toRawUTF8() << std::endl;
    }
    std::cout << std::endl;
}

//
//std::string toLower(std::string s)
//{
//    std::string s2 = s;
//
//    for (int i = 0; i < s.size(); i++) {
//        s2[i] = static_cast<char>(std::tolower(s2[i]));
//    }
//
//    return s2;
//}

void sendMessage(int device, int channel, int byte0, int byte1, int byte2)
{
    auto midiOutputDevices = juce::MidiOutput::getAvailableDevices();

    // check device
    if (midiOutputDevices.isEmpty())
    {
        std::cerr << "ERROR: No MIDI output devices found." << std::endl;
        exit(2);
    }

    // select device
    auto deviceInfo = midiOutputDevices[device - 1];
    auto midiOutput = juce::MidiOutput::openDevice(deviceInfo.identifier);
    if (midiOutput == nullptr)
    {
        std::cerr << "ERROR: Failed to open MIDI output device." << std::endl;
        exit(2);
    }

    // show dump
    std::stringstream hex0, hex1, hex2;
    hex0 << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << byte0 << "h";
    hex1 << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << byte1 << "h";
    hex2 << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << byte2 << "h";
    std::cout << std::endl;
    std::cout << "  device : " << device << " \"" << deviceInfo.name << "\"" << std::endl;
    std::cout << "  channel: " << (channel + 1) << std::endl;
    std::cout << "  bytes  : " << hex0.str() << " " << hex1.str() << " " << hex2.str();
    std::cout << " (" << byte0 << " " << byte1 << " " << byte2 << ")" << std::endl;
    std::cout << std::endl;
    std::cout << "  " << h.toString(byte0 & 0xF0, byte1, byte2) << std::endl;
    std::cout << std::endl;

    // send message
    auto msg = juce::MidiMessage(byte0, byte1, byte2);
    midiOutput->sendMessageNow(msg);
}
//
//
//bool isAlphabet(std::string s)
//{
//    std::regex re_command(R"(^[a-zA-Z][a-zA-Z0-9]*$)");
//    std::smatch match;
//
//    if (std::regex_match(s, match, re_command))
//    {
//        return true;
//    }
//    return false;
//}
//
//bool isInt1to16(std::string s)
//{
//    int n = getNumber(s);
//    if ((n < 1) || (n > 16))
//    {
//        return false;
//    }
//    return true;
//}
//
//bool isInt128(std::string s)
//{
//    int n = getNumber(s);
//    if ((n < 0) || (n > 127))
//    {
//        return false;
//    }
//    return true;
//}
//
//bool isInt256(std::string s)
//{
//    int n = getNumber(s);
//    if ((n < 0) || (n > 255))
//    {
//        return false;
//    }
//    return true;
//}
//
//int getNumber(std::string s, int defval)
//{
//    std::regex re_dec(R"(^\d?\d?\d$)");
//    std::regex re_hex1(R"(^[0-9a-fA-F]?[0-9a-fA-F]h$)");
//    std::regex re_hex2(R"(^0x[0-9a-fA-F]?[0-9a-fA-F]$)");
//    std::smatch match;
//    int value = defval;
//
//    if (std::regex_match(s, match, re_dec))
//    {
//        value = std::stoi(match[0].str());
//    }
//    else if (std::regex_match(s, match, re_hex1))
//    {
//        s.pop_back();
//        value = std::stoi(match[0].str(), nullptr, 16);
//    }
//    else if (std::regex_match(s, match, re_hex2))
//    {
//        s.erase(0, 2);
//        value = std::stoi(match[0].str(), nullptr, 16);
//    }
//
//    return value;
//}
//
