/*
  ==============================================================================

    LB_Synth_Gtr.h
    Created: 14 Nov 2022 11:13:05pm
    Author:  guita

  ==============================================================================
*/

#ifndef LB_SYNTH_GTR_H
#define LB_SYNTH_GTR_H

#include <vector>
#include <JuceHeader.h>

#include "LB_Osc_Gtr.h"

namespace LB {
    typedef unsigned int uint32;
    struct GtrSynthParams {
        float gain;
        float fbParam = 1;
        exciterType exciter = exciterType::plucked; //CHANGE
        float outGain = 1.f;
    };

    class Synth_Gtr {
    public:
        Synth_Gtr() = default;
        Synth_Gtr(const Synth_Gtr& synth) = default;
        ~Synth_Gtr() = default;

        void prepareToPlay(const uint32& sampleRate) {
            pluckedStrings.resize(128);
            // sinitialize string generator objects: set frequency, picked/plucked, load exciter signal
            for (int i = 0; i < pluckedStrings.size(); i++) {
                pluckedStrings[i].setParameters(midiToFreq(i), params.fbParam, params.exciter); //false bc plucked not picked
            }

            if (this->sampleRate != sampleRate) {
                for (int i = 0; i < pluckedStrings.size(); i++)
                    pluckedStrings[i].setSampleRate(sampleRate);
                //update other sample rates too
                this->sampleRate = sampleRate;
            }
        }

        void render(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& messages) {
            auto currentSample = 0;
            for (const auto midi : messages) {
                auto message = midi.getMessage();
                auto messagePosition = static_cast<int> (message.getTimeStamp());
                process(buffer, currentSample, messagePosition);
                currentSample = messagePosition;
                handleMidiEvent(message);
            }
            process(buffer, currentSample, buffer.getNumSamples());
        }

        void setParams(GtrSynthParams _params) {
            params = _params;
            // then set sub-object params
        }
    private:
        float midiToFreq(const uint32& midiNote) {
            constexpr float A4_FREQ = 440;
            constexpr float A4_MIDINOTE = 69;
            constexpr float NOTES_IN_OCTAVE = 12.f;
            return A4_FREQ * std::powf(2, (static_cast<float>(midiNote) - A4_MIDINOTE) / NOTES_IN_OCTAVE);
        }

        void handleMidiEvent(const juce::MidiMessage& message) {
            if (message.isNoteOn()) {
                // make sure note is not too low to be stored in a 1024 sample circular buffer at 48khz sample rate
                if (message.getNoteNumber() > 31)
                    pluckedStrings[message.getNoteNumber()].start(message.getVelocity());
            }

            if (message.isNoteOff()) {
                pluckedStrings[message.getNoteNumber()].stop();
            }

            if (message.isAllNotesOff()) {
                for (int i = 0; i < pluckedStrings.size(); i++)
                    pluckedStrings[i].stop();
            }
        }

        void process(juce::AudioBuffer<float>& buffer, const uint32& startSample, const uint32& endSample) {
            auto* firstChannel = buffer.getWritePointer(0);
            
            //update plucked strings parameters
            for (int i = 0; i < pluckedStrings.size(); i++) {
                pluckedStrings[i].setParameters(midiToFreq(i), params.fbParam, params.exciter); //false bc plucked not picked
            }

            for (int i = 0; i < pluckedStrings.size(); i++) {
                //if (pluckedStrings[i].isPlaying()) {
                    for (auto j = startSample; j < endSample; j++) {
                        auto sample = pluckedStrings[i].getSample() * params.outGain;
                        firstChannel[j] += sample;
                    }
                //}
            }

            for (uint32 channel = 1; channel < buffer.getNumChannels(); channel++) {
                auto channelData = buffer.getWritePointer(channel);
                std::copy(firstChannel + startSample, firstChannel + endSample, channelData + startSample);
            }
        }

        uint32 sampleRate;
        std::vector<LB::GtrString> pluckedStrings;
        GtrSynthParams params;

    };



}

#endif
