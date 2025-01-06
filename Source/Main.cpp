// mider
//   Description: simple command-line MIDI messaging tool
//   Author: aike
//   License: MIT

constexpr auto VERSION = "1.0.1";

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
void sendMessage(int device, std::vector<uint8_t>data);
void receiveMessage(int device);
void help(P htype);

Help h;
ArgParser parser;
MidiReceiver receiver;

constexpr auto BUFMAX = 4096;
uint8_t msgbuf[BUFMAX];

/////////// TEST /////////////
//#define TEST
#include "Test.h"
//////////////////////////////

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
    case P::SHOWVERSION:
        std::cout << "mider " << VERSION << std::endl;
        break;
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
    case P::DEV_CH_CCX_CHANNELVOICEMSG:
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
    case P::HELP_MIDERCOMMAND:
    case P::HELP_MSGNAME:
    case P::HELP_CC:
    case P::HELP_CC_CCNAME:
    case P::HELP_CM:
    case P::HELP_CM_CMNAME:
        help(state);
        break;

    case P::E_SYNTAX_ERROR:
    case P::E_MSGNAME_ERROR:
    case P::E_CCNAME_ERROR:
    case P::E_ARG_ERROR:
    case P::E_MSGBUF_ERROR:
        std::cerr << parser.getText() << std::endl;
        return state - P::ERROR;
        break;
    default:
        break;
    }

    return 0;
}
#endif


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

void usage(void)
{
    std::cout << std::endl;
    std::cout << "mider ver " << VERSION << std::endl;
    std::cout << "  simple command-line MIDI messaging tool" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  List MIDI Devices:" << std::endl;
    std::cout << "    mider [devices|indevices|outdevices]" << std::endl;
    std::cout << std::endl;
    std::cout << "  Send MIDI Message:" << std::endl;
    std::cout << "    mider <dev_no> <ch_no> <message_name> [byte ...]" << std::endl;
    std::cout << "    mider <dev_no> <message_name> [byte ...]" << std::endl;
    std::cout << "    mider <dev_no> <ch_no> channelmode <channelmode_message_name> <byte>" << std::endl;
    std::cout << "    mider <dev_no> <ch_no> controlchange <controlchange_name> <byte>" << std::endl;
    std::cout << "    mider <dev_no> <ch_no> controlchange <controlchange_name> <msb> <lsb>" << std::endl;
    std::cout << "    mider <dev_no> sox [byte ...] eox" << std::endl;
    std::cout << "    mider <dev_no> <byte> [byte ...]" << std::endl;
    std::cout << std::endl;
    std::cout << "  Receive MIDI Message:" << std::endl;
    std::cout << "    mider <dev_no> receive" << std::endl;
    std::cout << std::endl;
    std::cout << "  Show Help:" << std::endl;
    std::cout << "    mider version" << std::endl;
    std::cout << "    mider help" << std::endl;
    std::cout << "    mider help <message_name>" << std::endl;
    std::cout << "    mider help controlchange" << std::endl;
    std::cout << "    mider help controlchange <controlchange_name>" << std::endl;
    std::cout << "    mider help channelmode" << std::endl;
    std::cout << "    mider help channelmode <channelmode_message_name>" << std::endl;
    std::cout << "    mider help [receive|devices|indevices|outdevices|version]" << std::endl;
    std::cout << std::endl;
    std::cout << "  Example:" << std::endl;
    std::cout << "    mider version" << std::endl;
    std::cout << "    mider devices" << std::endl;
    std::cout << "    mider 1 receive" << std::endl;
    std::cout << "    mider 1 1 NoteOn 60 100" << std::endl;
    std::cout << "    mider 1 1 noteon 0x3C 0x64" << std::endl;
    std::cout << "    mider 1 90h 3Ch 64h" << std::endl;
    std::cout << "    mider 1 16 ControlChange BankSelectMSB 10" << std::endl;
    std::cout << "    mider 1 16 CC BankSelectMSB 10" << std::endl;
    std::cout << "    mider 1 16 BankSelectMSB 10" << std::endl;
    std::cout << "    mider 1 16 BSM 10" << std::endl;
    std::cout << "    mider 1 16 CC BankSelect 10 20" << std::endl;
    std::cout << "    mider 1 16 BankSelect 10 20" << std::endl;
    std::cout << "    mider 1 16 BS 10 20 " << std::endl;
    std::cout << "    mider 1 5 ChannelMode AllNotesOff 0" << std::endl;
    std::cout << "    mider 1 5 CM AllNotesOff 0" << std::endl;
    std::cout << "    mider 1 5 AllNotesOff 0" << std::endl;
    std::cout << "    mider 1 5 ANO 0" << std::endl;
    std::cout << "    mider 1 TuneRequest" << std::endl;
    std::cout << "    mider 1 Start" << std::endl;
    std::cout << "    mider 1 SOX 7Dh 01h 02h 03h 04h EOX" << std::endl;
    std::cout << "    mider help" << std::endl;
    std::cout << "    mider help noteon" << std::endl;
    std::cout << "    mider help cc" << std::endl;
    std::cout << "    mider help cc bankselect" << std::endl;
    std::cout << "    mider help cm" << std::endl;
    std::cout << "    mider help cm allnotesoff" << std::endl;
    std::cout << std::endl;
}

void help(P htype)
{
    switch (htype)
    {
    case P::HELP:
        std::cout << std::endl;
        std::cout << "Message Help:" << std::endl;
        for (int i = 0; i < 256; i++)
        {
            if (i == 0x80)
            {
                std::cout << "  Channel Voice Message:" << std::endl;
            }
            else if (i == 0xF0)
            {
                std::cout << "  Channel Mode Message:" << std::endl;
                for (int j = 0x78; j < 0x80; j++)
                {
                    if (h.ccCommandName(j) != "Undefined")
                    {
                        std::cout << "    B0h Channel Mode Message    | " << h.ccCommandHelp(j, 21) << std::endl;
                    }
                }

                std::cout << "  System Common Message:" << std::endl;
            }
            else if (i == 0xF8)
            {
                std::cout << "  System Realtime Message:" << std::endl;
            }
            if (h.commandName(i) != "Undefined")
            {
                std::cout << "    " << h.commandHelp(i, 23, 26) << std::endl;
            }
        }
        std::cout << std::endl;
        break;

    case P::HELP_MIDERCOMMAND:
        std::cout << std::endl;
        std::cout << "Mider Command Help:" << std::endl;
        std::cout << "  " << parser.getText() << std::endl;
        std::cout << std::endl;
        break;
    case P::HELP_MSGNAME:
        std::cout << std::endl;
        std::cout << "Message Help:" << std::endl;
        std::cout << "  " << parser.getText() << std::endl;
        std::cout << std::endl;
        break;

    case P::HELP_CC:
        std::cout << std::endl;
        std::cout << "Control Change Help:" << std::endl;
        for (int i = 0; i < 0x78; i++)
        {
            if (h.ccCommandName(i) != "Undefined")
            {
                std::cout << "  B0h Control Change | " << h.ccCommandHelp(i, 31) << std::endl;
            }
        }
        std::cout << std::endl;
        break;

    case P::HELP_CC_CCNAME:
        std::cout << std::endl;
        std::cout << "Control Change Help:" << std::endl;
        std::cout << "  B0h Control Change | " << parser.getText() << std::endl;
        std::cout << std::endl;
        break;

    case P::HELP_CM:
        std::cout << std::endl;
        std::cout << "Channel Mode Message Help:" << std::endl;
        for (int i = 0x78; i < 0x80; i++)
        {
            if (h.ccCommandName(i) != "Undefined")
            {
                std::cout << "  B0h Channel Mode Message | " << h.ccCommandHelp(i, 21) << std::endl;
            }
        }
        std::cout << std::endl;
        break;

    case P::HELP_CM_CMNAME:
        std::cout << std::endl;
        std::cout << "Channel Mode Message Help:" << std::endl;
        std::cout << "  B0h Channel Mode | " << parser.getText() << std::endl;
        std::cout << std::endl;
        break;

    }
}
