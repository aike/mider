/*
  ==============================================================================

    MidiReceiver.cpp
    Author:  aike

  ==============================================================================
*/


#include "MidiReceiver.h"

MidiReceiver::MidiReceiver()
{

}

MidiReceiver::~MidiReceiver()
{
    if (midiInput)
    {
        midiInput->stop();
    }
}

void MidiReceiver::handleIncomingMidiMessage(juce::MidiInput*, const juce::MidiMessage& message)
{
    const uint8_t* u8data = message.getRawData();
    int size = message.getRawDataSize();
    // uint8_t[] を std::vector<uint8_t> に変換
    std::vector<uint8_t> data(u8data, u8data + size);

    std::stringstream dec;
    std::stringstream hex;
    for (int i = 0; i < size; i++)
    {
        dec << std::to_string(data[i]);
        hex << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (int)data[i] << "h";
        if (i < size - 1)
        {
            dec << " ";
            hex << " ";
        }
    }

    // show dump
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
}

int MidiReceiver::start(int device)
{
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    if (midiInputs.isEmpty())
    {
        std::cerr << "No MIDI input devices available." << std::endl;
        return -1;
    }

    // 最初のMIDI入力デバイスを開く
    auto deviceInfo = midiInputs[device - 1];
    //std::cout << "Opening MIDI input: " << deviceInfo.name << std::endl;

    midiInput = juce::MidiInput::openDevice(deviceInfo.identifier, this);

    if (midiInput)
    {
        deviceno = device;
        devicename = deviceInfo.name.toStdString();;
        std::cout << "Waiting MIDI Message..." << std::endl;
        std::cout << "  device : " << deviceno << " \"" << devicename << "\"" << std::endl;
        std::cout << std::endl;
        midiInput->start();
    }
    else
    {
        std::cerr << "Failed to open MIDI input." << std::endl;
        return -2;
    }

    return 0;
}
