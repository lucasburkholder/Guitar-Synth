/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarSynthAudioProcessorEditor::GuitarSynthAudioProcessorEditor (GuitarSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (WIDTH, HEIGHT);

    fbKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    fbKnob.setRange(0, 100, 1);
    fbKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 20);
    fbKnob.setValue(90);
    fbKnob.setTextValueSuffix("%");
    addAndMakeVisible(&fbKnob);

    fbKnob.onValueChange = [this] {
        audioProcessor.fbParam = fbKnob.getValue() / 100.f;
    };

    volumeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeKnob.setRange(-48.0, 12, 0.1);
    volumeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 20);
    volumeKnob.setValue(0);
    volumeKnob.setTextValueSuffix("dB");
    addAndMakeVisible(&volumeKnob);

    volumeKnob.onValueChange = [this] {
        audioProcessor.outGain = pow(10, volumeKnob.getValue() * 0.05);
    };

    titleLabel.setFont(juce::Font(18.f, juce::Font::bold));
    titleLabel.setText("G U I T A R S Y N T H", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&titleLabel);

    fbKnobLabel.setFont(juce::Font (14.0f));
    fbKnobLabel.setText("Length", juce::dontSendNotification);
    fbKnobLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&fbKnobLabel);

    volumeKnobLabel.setFont(juce::Font(14.f));
    volumeKnobLabel.setText("Volume", juce::dontSendNotification);
    volumeKnobLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&volumeKnobLabel);
}

GuitarSynthAudioProcessorEditor::~GuitarSynthAudioProcessorEditor()
{
}

//==============================================================================
void GuitarSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    
}

void GuitarSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    titleLabel.setBounds(0, outerMargin, WIDTH, titleHeight);

    fbKnob.setBounds(outerMargin, outerMargin+margin+titleHeight+labelHeight, knobWidth, knobHeight);
    fbKnobLabel.setBounds(outerMargin, outerMargin+margin+titleHeight, knobWidth, labelHeight);

    volumeKnob.setBounds(outerMargin + margin + knobWidth, outerMargin+margin+titleHeight+labelHeight, knobWidth, knobHeight);
    volumeKnobLabel.setBounds(outerMargin+margin+knobWidth, outerMargin + margin + titleHeight, knobWidth, labelHeight);
    
}
