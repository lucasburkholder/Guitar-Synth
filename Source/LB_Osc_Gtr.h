/*
  ==============================================================================

    LB_Osc_Gtr.h
    Created: 14 Nov 2022 11:13:38pm
    Author:  guita

  ==============================================================================
*/

#ifndef LB_OSC_GTR_H
#define LB_OSC_GTR_H

#include <cstdlib>

#include "LB_Exciters.h"
#include "LB_waveguide.h"

namespace LB {
    typedef unsigned int uint32;
    typedef unsigned char uint8;

    enum class exciterType {plucked, picked, noise};

    class GtrString {
    public:
        GtrString() = default;
        GtrString(GtrString&) = default;
        ~GtrString() = default;

        void setSampleRate(const uint32& sampleRate) {
            if (this->sampleRate != sampleRate) {
                this->sampleRate = sampleRate;
                //UPDATE OTHER OBJECT'S SAMPLE RATES
                adsr.setSampleRate(sampleRate);
                stop();
            }
        }

        float getSample() {
            float adsrSample = adsr.getNextSample();
            double exciterSample = 0.0;
            if (playing) {
                
                switch (exciterType) {
                case exciterType::noise:
                    exciterSample = noiseExciter.getSample();
                case exciterType::picked:
                    exciterSample = pickExciter.getSample();
                case exciterType::plucked:
                    exciterSample = pluckExciter.getSample();
                }
                
            } 

            double delayFeedback_filtered = loopFilter.processSample(delayOutput);

            //float mappedFBGain = 0.8 + 0.2 * feedbackParameter;
            delayOutput = delayLine.processSample(exciterSample + delayFeedback_filtered * mappedFBGain); //feedback in here
            sample = delayOutput * noteGainFactor * adsrSample;
            return sample;
        }



        void start(uint8 velocity) { //equivalent of setFrequency() in G-funk synth
            //convert velocity to gain
            noteGainFactor = (float)velocity / 127.f;
            juce::ADSR::Parameters adsrParams;
            adsrParams.attack = 0.01;
            adsrParams.decay = 1.0;
            adsrParams.sustain = 1.0;
            adsrParams.release = 0.2;
            adsr.setParameters(adsrParams);
            adsr.reset();
            adsr.noteOn();

            delayLine.clearBuffer();
            delayLine.init(sampleRate / frequency); 
            loopFilter.resetDelayRegisters();

            switch (exciterType) {
            case exciterType::noise:
                noiseExciter.startFromBeginning();
            case exciterType::picked:
                pickExciter.startFromBeginning();
            case exciterType::plucked:
                pluckExciter.startFromBeginning();
            }

            playing = true;
        }

        void stop() {
            playing = false;
            adsr.noteOff();
        }

        bool isPlaying() { return playing; }

        void setGainFactor(float f) { noteGainFactor = f; }

        void setParameters(float freq, float fbParam, exciterType exciter) {
            frequency = freq;
            exciterType = exciter; 
            feedbackParameter = fbParam;

            // map fb gain param
            const float b = 1.6180339887498948482045868343656;
            //mappedFBGain = 0.8 + 0.2 * fbParam;
            mappedFBGain = b - 1.f / (pow((fbParam - 1.f), 3) + b); // the new mapping function
        }
    private:
        uint32 sampleRate; //MAKE SURE TO INITIALIZE
        float noteGainFactor; //MAKE SURE TO INITIALIZE

        // parameters
        float frequency;
        exciterType exciterType = exciterType::plucked;
        bool playing;
        float feedbackParameter = 1.f;
        float mappedFBGain = 1.f;

        // Exciter/Waveguide objects
        NoiseExciter noiseExciter;
        PluckExciter pluckExciter;
        PickExciter pickExciter;
        FractionalDelayLine delayLine;
        LoopFilterGtr loopFilter;

        // other variables
        float sample = 0;
        double delayOutput = 0.0; //output to feed back in
        juce::ADSR adsr;
    };
}

#endif