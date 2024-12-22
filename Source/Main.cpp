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
int getNumber(std::string s);
std::string toLower(std::string s);
void sendMessage(int device, int channel, int byte0, int byte1, int byte2);

//==============================================================================
int main (int argc, char* argv[])
{
    if (argc < 2) { usage(); }

    std::string arg = argv[1];
    if ((argc == 2) && (arg == "list"))
    {
        listDevices();
        return 0;
    }

    // 正規表現パターン
    std::regex re_number(R"(\d?\d?\d)");
    std::regex re_hex(R"(\d?\d?\dh)");
    //std::regex re_command(R"([a-z]+)");
    std::smatch match;

    if (argc < 4) { usage(); }

    int device = 0;
    int ch = 0;

    // Device
    arg = argv[1];
    if (std::regex_match(arg, match, re_number))
    {
        device = std::stoi(match[0].str());
    }
    else { usage(); }

    // Channel
    arg = argv[2];
    if (std::regex_match(arg, match, re_number))
    {
        ch = std::stoi(match[0].str());
    }
    else { usage(); }

    // Command
    std::string cmd[3];
    cmd[0] = toLower(argv[3]);
    cmd[1] = (argc > 4) ? argv[4] : "0";
    cmd[2] = (argc > 5) ? argv[5] : "0";
    int byte1 = getNumber(cmd[1]);
    int byte2 = getNumber(cmd[2]);

    if (cmd[0] == "noteon")
    {
        sendMessage(device, ch, 0x90 + ch, byte1, byte2);
        std::cout << "  NoteOn";
        std::cout << " " << byte1 << " " << byte2;
    }
    else if (cmd[0] == "noteoff")
    {
        sendMessage(device, ch, 0x80 + ch, byte1, byte2);
        std::cout << "  NoteOff";
        std::cout << " " << byte1 << " " << byte2;
    }
    else if (cmd[0] == "allsoundoff")
    {
        // TODO: CCとしての書き方検討
        // check argv[4]が省略または120であること
        // check argv[5]が省略または0であること

        sendMessage(device, ch, 0xB0 + ch, 120, byte2);
        std::cout << "  AllSoundOff";
        std::cout << " " << 120 << " " << byte2;
    }
    else if (cmd[0] == "allnotesoff")
    {
        // TODO: CCとしての書き方検討
        // check argv[4]が省略または123であること
        // check argv[5]が省略または0であること

        sendMessage(device, ch, 0xB0 + ch, 123, byte2);
        std::cout << "  AllNotesOff";
        std::cout << " " << 123 << " " << byte2;
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

int getNumber(std::string s)
{
    std::regex re_dec(R"(\d?\d?\d)");
    std::regex re_hex(R"(\d?\d?\dh)");
    std::smatch match;
    int value = 0;

    if (std::regex_match(s, match, re_dec))
    {
        value = std::stoi(match[0].str());
    }
    else if (std::regex_match(s, match, re_hex))
    {
        s.pop_back();
        value = std::stoi(match[0].str(), nullptr, 16);
    }

    if (value > 127)
    {
        std::cerr << "ERROR; number must be 0..127";
        exit(1);
    }

    return value;
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
