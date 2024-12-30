/*
  ==============================================================================

    MidiReceiver.cpp
    Created: 30 Dec 2024 11:58:58pm
    Author:  ai

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
    std::cout << "  Received MIDI message: " << message.getDescription() << std::endl;
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
    auto& deviceInfo = midiInputs[device - 1];
    std::cout << "Opening MIDI input: " << deviceInfo.name << std::endl;

    midiInput = juce::MidiInput::openDevice(deviceInfo.identifier, this);

    if (midiInput)
    {
        midiInput->start();
    }
    else
    {
        std::cerr << "Failed to open MIDI input." << std::endl;
        return -2;
    }

    return 0;
}
