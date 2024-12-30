/*
  ==============================================================================

    MidiReceiver.h
    Created: 30 Dec 2024 11:58:58pm
    Author:  ai

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MidiReceiver : public juce::MidiInputCallback
{
public:
    MidiReceiver();
    ~MidiReceiver();
    void handleIncomingMidiMessage(juce::MidiInput*, const juce::MidiMessage& message) override;
    int start(int device);

private:
    std::unique_ptr<juce::MidiInput> midiInput;
};
