/*
  ==============================================================================

    LB_Exciters.h
    Created: 15 Nov 2022 4:24:53pm
    Author:  guita

  ==============================================================================
*/

#ifndef LB_EXCITERS_H
#define LB_EXCITERS_H

#include <cstdlib>
#include <JuceHeader.h>

namespace LB {

    /*
        Excitation signal generator objects
            - start from beginning, outputting samples sequentially every time
                getSample() is called.
            - There is no stop method. Once finished playing, they output silence
                when getSample() is called.
    */
    class NoiseExciter { // 100 samples of white noise
    public:
        NoiseExciter() : numSamples(100), currentSample(0) { 
            samples = new double[numSamples]; //allocate memory
            //load samples (can maybe just use memcpy for the audio files?)
            for (int i = 0; i < numSamples; i++)
                samples[i] = static_cast<double> (std::rand()) / static_cast<double> (RAND_MAX) - 0.5;
        }
        ~NoiseExciter() { delete[] samples; }

        void startFromBeginning() {
            currentSample = 0;
        }

        double getSample() {
            currentSample++; //increment index

            //If current sample past end of array, keep it there and return 0.
            //Otherwise, return current sample
            if (currentSample >= numSamples) { //if we are past the end of the array
                currentSample = numSamples; //keep index past the end of the array
                return 0;
            }
            else return samples[currentSample];
        }
    private:
        float numSamples;
        double* samples;
        int currentSample; //index of current sample we are outputting
    };

    class PluckExciter {
    public:
        PluckExciter() : numSamples(1000), currentSample(0) {
            // load in file and do the other stuff
            juce::AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            juce::File exciterFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile("source/a4 exciter.wav");
            juce::AudioFormatReader* reader = formatManager.createReaderFor(exciterFile);
            numSamples = reader->lengthInSamples;

            juce::AudioSampleBuffer buffer;
            buffer.setSize(reader->numChannels, reader->lengthInSamples, false, true);
            reader->read(&buffer, 0, reader->lengthInSamples, 0, true, true);
            numSamples = reader->lengthInSamples;
            delete reader;

            samples = new double[numSamples];
            const float * readPtr = buffer.getReadPointer(0); // maybe channel 1 instead?
            for (int i = 0; i < numSamples; i++)
                samples[i] = readPtr[i];
            
        }
        
        ~PluckExciter() {
            delete[] samples;
        }
        void startFromBeginning() {
            currentSample = 0;
        }
        double getSample() {
            currentSample++; 

            if (currentSample >= numSamples) {
                currentSample = numSamples;
                return 0;
            }
            else return samples[currentSample];
        }
    private:
        float numSamples;
        double* samples;
        int currentSample;

        
    };

    class PickExciter {
    public:
        PickExciter() : numSamples(1000), currentSample(0) {
            // load in file and do the other stuff
            juce::AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            juce::File exciterFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile("source/370hz exciter picked.wav");
            juce::AudioFormatReader* reader = formatManager.createReaderFor(exciterFile);
            numSamples = reader->lengthInSamples;

            juce::AudioSampleBuffer buffer;
            buffer.setSize(reader->numChannels, reader->lengthInSamples, false, true);
            reader->read(&buffer, 0, reader->lengthInSamples, 0, true, true);
            numSamples = reader->lengthInSamples;
            delete reader;

            samples = new double[numSamples];
            const float* readPtr = buffer.getReadPointer(0); // maybe channel 1 instead?
            for (int i = 0; i < numSamples; i++)
                samples[i] = readPtr[i];

        }

        ~PickExciter() {
            delete[] samples;
        }
        void startFromBeginning() {
            currentSample = 0;
        }
        double getSample() {
            currentSample++;

            if (currentSample >= numSamples) {
                currentSample = numSamples;
                return 0;
            }
            else return samples[currentSample];
        }
    private:
        float numSamples;
        double* samples;
        int currentSample;


    };
}
#endif