/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GuitarSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GuitarSynthAudioProcessorEditor (GuitarSynthAudioProcessor&);
    ~GuitarSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuitarSynthAudioProcessor& audioProcessor;

    juce::Slider fbKnob;
    juce::Slider volumeKnob;
    
    juce::Label fbKnobLabel;
    juce::Label volumeKnobLabel;
    juce::Label titleLabel;

    

    int knobWidth = 100;
    int knobHeight = 100;
    int labelHeight = 20;

    int margin = 5;
    int outerMargin = 20;

    int titleHeight = 20;

    int WIDTH = 2*outerMargin + margin + 2 * knobWidth;
    int HEIGHT = titleHeight + 2*outerMargin + margin + knobHeight + labelHeight;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarSynthAudioProcessorEditor)
};
