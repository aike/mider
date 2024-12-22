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

void usage(void);
void listDevices(void);
std::string toLower(std::string s);
void sendMessage(int device, int channel, int byte0, int byte1, int byte2);

bool isAlphabet(std::string s);
bool isInt128(std::string s);
bool isInt256(std::string s);
int getNumber(std::string s);


//==============================================================================
int main (int argc, char* argv[])
{
    // convert from char* array to std::string array
    std::string arg[10];
    if (argc > 10)
    {
        argc = 10;
    }
    for (int i = 0; i < argc; i++)
    {
        arg[i] = argv[i];
    }
    for (int i = argc; i < 10; i++)
    {
        arg[i] = "";
    }


    ////// (1) mider ////////
    if (arg[1] == "") { usage(); }


    ////// (2) mider list ////////
    if ((arg[1] == "list") && (arg[2] == ""))
    {
        listDevices();
        return 0;
    }
    else if ((arg[1] == "help") && (arg[2] == ""))
    {
        listDevices();
        return 0;
    }

    int device = 0;
    int ch = 0;
    std::string cmd;
    int byte1 = 0;
    int byte2 = 0;

    ////// (3) mider device port "cc" cc_command byte2 ////////
    if (isInt128(arg[1]) && isInt128(arg[2]) && (arg[3] == "cc") && isAlphabet(arg[4]))
    {
        device = std::stoi(arg[1]);
        ch = std::stoi(arg[2]);
        cmd = toLower(arg[4]);
        byte2 = getNumber(arg[5]);

        if (cmd == "allsoundoff")
        {
            int code = 120;
            sendMessage(device, ch, 0xB0 + ch, code, byte2);
            std::cout << "  AllSoundOff";
            std::cout << " " << code << " " << byte2;
        }
        else if (cmd == "allnotesoff")
        {
            int code = 123;
            sendMessage(device, ch, 0xB0 + ch, code, byte2);
            std::cout << "  AllSoundOff";
            std::cout << " " << code << " " << byte2;
        }
    }

    ////// (4) mider device port command byte1 byte2 ////////
    if (isInt128(arg[1]) && isInt128(arg[2]) && isAlphabet(arg[3]))
    {
        device = std::stoi(arg[1]);
        ch = std::stoi(arg[2]);
        cmd = toLower(arg[3]);
        byte1 = getNumber(arg[4]);
        byte2 = getNumber(arg[5]);

        if (cmd == "noteon")
        {
            sendMessage(device, ch, 0x90 + ch, byte1, byte2);
            std::cout << "  NoteOn";
            std::cout << " " << byte1 << " " << byte2;
        }
        else if (cmd == "noteoff")
        {
            sendMessage(device, ch, 0x80 + ch, byte1, byte2);
            std::cout << "  NoteOff";
            std::cout << " " << byte1 << " " << byte2;
        }
    }

    std::cout << std::endl;

    return 0;
}

void usage(void)
{
    std::cout << "usage: mider dev ch message byte1 byte2";
    std::cout << "       mider list";
    exit(0);
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


std::string toLower(std::string s)
{
    std::string s2 = s;

    for (int i = 0; i < s.size(); i++) {
        s2[i] = static_cast<char>(std::tolower(s2[i]));
    }

    return s2;
}

void sendMessage(int device, int channel, int byte0, int byte1, int byte2)
{
    auto midiOutputDevices = juce::MidiOutput::getAvailableDevices();

    if (midiOutputDevices.isEmpty())
    {
        std::cerr << "ERROR: No MIDI output devices found." << std::endl;
        exit(1);
    }

    auto deviceInfo = midiOutputDevices[device - 1];
    auto midiOutput = juce::MidiOutput::openDevice(deviceInfo.identifier);
    if (midiOutput == nullptr)
    {
        std::cerr << "ERROR: Failed to open MIDI output device." << std::endl;
        exit(1);
    }

    std::stringstream hex0, hex1, hex2;
    hex0 << std::hex << std::uppercase << byte0 << "h";
    hex1 << std::hex << std::uppercase << byte1 << "h";
    hex2 << std::hex << std::uppercase << byte2 << "h";

    std::cout << "  device : " << device << " " << deviceInfo.name << std::endl;
    std::cout << "  channel: " << channel << std::endl;
    std::cout << "  bytes  : " << hex0.str() << " " << hex1.str() << " " << hex2.str();
    std::cout << " (" << byte0 << " " << byte1 << " " << byte2 << ")" << std::endl;

    int msgtype = byte0 & 0xF0;

    if (msgtype == 0x90)      // Note On
    {
        auto msg = juce::MidiMessage::noteOn(channel, byte1, (juce::uint8)byte2);
        midiOutput->sendMessageNow(msg);
    }
    else if (msgtype == 0x80) // Note Off
    {
        auto msg = juce::MidiMessage::noteOff(channel, byte1, (juce::uint8)byte2);
        midiOutput->sendMessageNow(msg);
    }
    else if (msgtype == 0xB0) // CC
    {
        if (byte1 == 120) // All Sound Off
        {
            auto msg = juce::MidiMessage::allSoundOff(channel);
            midiOutput->sendMessageNow(msg);
        }
        else if (byte1 == 123) // All Notes Off
        {
            auto msg = juce::MidiMessage::allNotesOff(channel);
            midiOutput->sendMessageNow(msg);
        }
    }
}



bool isAlphabet(std::string s)
{
    std::regex re_command(R"([a-zA-Z]+)");
    std::smatch match;

    if (std::regex_match(s, match, re_command))
    {
        return true;
    }
    return false;
}

bool isInt128(std::string s)
{
    int n = getNumber(s);
    if ((n < 0) || (n > 127))
    {
        return false;
    }
    return true;
}

bool isInt256(std::string s)
{
    int n = getNumber(s);
    if ((n < 0) || (n > 255))
    {
        return false;
    }
    return true;
}

int getNumber(std::string s)
{
    std::regex re_dec(R"(\d?\d?\d)");
    std::regex re_hex(R"(\d?\d?\dh)");
    std::smatch match;
    int value = -1;

    if (std::regex_match(s, match, re_dec))
    {
        value = std::stoi(match[0].str());
    }
    else if (std::regex_match(s, match, re_hex))
    {
        s.pop_back();
        value = std::stoi(match[0].str(), nullptr, 16);
    }

    return value;
}

