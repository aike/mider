/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

// TODO
// - help
// - sysex
// - readme
// - source header comment
// - mac build
// 
// - ok cc/cmの省略
// - cc cccommand msb lsbで2メッセージ送信
// - 複数メッセージ送信 NoteOn 60 100 NoteOn 64 100
// - running status

#include <JuceHeader.h>
#include <regex>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <csignal>


#include "Help.h"
#include "ArgParser.h"
#include "MidiReceiver.h"

void usage(void);
void listInDevices(void);
void listOutDevices(void);
void listAllDevices(void);
//void sendMessage(int device, int channel, int byte0, int byte1, int byte2);
void sendMessage(int device, std::vector<uint8_t>data);
void receiveMessage(int device);


Help h;
ArgParser parser;
MidiReceiver receiver;

constexpr auto BUFMAX = 4096;
uint8_t msgbuf[BUFMAX];

//#define TEST

#include "Test.h"


// Ctrl+Cを検知したときにtrueにするフラグ
volatile std::sig_atomic_t keepRunning = 1;

// Ctrl+Cのハンドラ
void signalHandler(int)
{
    keepRunning = 0;
}


//==============================================================================
#ifndef TEST
int main (int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    P state = parser.parse(args);

    switch (state)
    {
    case P::DEVICE:
        listAllDevices();
        std::cout << std::endl;
        break;
    case P::INDEVICE:
        listInDevices();
        std::cout << std::endl;
        break;
    case P::OUTDEVICE:
        listOutDevices();
        std::cout << std::endl;
        break;

    case P::DEV_CH_CHANNELVOICEMSG:
    case P::DEV_CH_CC_CHANNELVOICEMSG:
    case P::DEV_SYSTEMCOMMONMSG:
    case P::DEV_SYSTEMRTMSG:
    case P::DEV_BYTELIST:
        sendMessage(parser.getDevice(), parser.getBytes());
        break;
 
    case P::DEV_RECEIVE:
        receiveMessage(parser.getDevice());
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
    case P::E_MSGNAME_ERROR:
    case P::E_CCNAME_ERROR:
    case P::E_ARG_ERROR:
        std::cerr << parser.getText();
        return state - P::ERROR;
        break;
    default:
        break;
    }

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

void listInDevices(void)
{
    // List Output Devices
    auto midiOutputDevices = juce::MidiInput::getAvailableDevices();
    std::cout << std::endl;
    std::cout << "MIDI Input Devices:" << std::endl;
    for (int i = 0; i < midiOutputDevices.size(); i++)
    {
        const auto& device = midiOutputDevices[i];
        std::cout << "  " << (i + 1) << " " << device.name.toRawUTF8() << std::endl;
    }
}

void listOutDevices(void)
{
    // List Output Devices
    auto midiOutputDevices = juce::MidiOutput::getAvailableDevices();
    std::cout << std::endl;
    std::cout << "MIDI Output Devices:" << std::endl;
    for (int i = 0; i < midiOutputDevices.size(); i++)
    {
        const auto& device = midiOutputDevices[i];
        std::cout << "  " << (i + 1) << " " << device.name.toRawUTF8() << std::endl;
    }
}

void listAllDevices(void)
{
    listInDevices();
    listOutDevices();
}




void sendMessage(int device, std::vector<uint8_t>data)
{
    auto midiOutputDevices = juce::MidiOutput::getAvailableDevices();

    // check device
    if (midiOutputDevices.isEmpty())
    {
        std::cerr << "ERROR: No MIDI output devices found." << std::endl;
        exit(P::E_MIDI_DEVICE_ERROR - P::ERROR);
    }

    // select device
    auto deviceInfo = midiOutputDevices[device - 1];
    auto midiOutput = juce::MidiOutput::openDevice(deviceInfo.identifier);
    if (midiOutput == nullptr)
    {
        std::cerr << "ERROR: Failed to open MIDI output device." << std::endl;
        exit(P::E_MIDI_DEVICE_ERROR - P::ERROR);
    }

    std::stringstream dec;
    std::stringstream hex;
    for (int i = 0; i < data.size(); i++)
    {
        dec << std::to_string(data[i]);
        hex << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (int)data[i] << "h";
        if (i < data.size() - 1)
        {
            dec << " ";
            hex << " ";
        }
    }

    // show dump
    std::cout << std::endl;
    std::cout << "  device : " << device << " \"" << deviceInfo.name << "\"" << std::endl;
    MSG msgtype = h.getMessageType(data);
    if ((msgtype == MSG::ChannelVoice) || (msgtype == MSG::ChannelVoiceCc) || (msgtype == MSG::ChannelMode))
    {
        int ch = (int)data[0] & 0x0F;
        std::cout << "  channel: " << (ch + 1) << std::endl;
    }
    std::cout << "  bytes  : " << hex.str() << " (" << dec.str() << ")" << std::endl;
    std::cout << "  type   : " << h.getMessageTypeName(msgtype) << std::endl;
    std::cout << "  message: " << h.toString(data) << std::endl;
    std::cout << std::endl;

    // send message
    if (data.size() >= BUFMAX)
    {
        std::cerr << "ERROR: Message buffer overflow." << std::endl;
        exit(P::E_MSGBUF_ERROR - P::ERROR);
    }
    std::copy(data.begin(), data.end(), msgbuf);
    auto msg = juce::MidiMessage(msgbuf, (int)data.size());
    midiOutput->sendMessageNow(msg);
}

void receiveMessage(int device)
{
    // Ctrl+C シグナルハンドラを登録
    std::signal(SIGINT, signalHandler);

    //std::cout << "Starting MIDI Receiver..." << std::endl;
    receiver.start(device);
    //std::cout << "Press Ctrl+C to exit." << std::endl;

    // Ctrl+Cが押されるまで待機
    while (keepRunning)
    {
        juce::Thread::sleep(100); // 少し待機してCPU負荷を軽減
    }

    std::cout << "Exiting MIDI Receiver." << std::endl;

    exit(0);
}

