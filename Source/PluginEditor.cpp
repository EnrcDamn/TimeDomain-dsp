/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TimeDomainTestingAudioProcessorEditor::TimeDomainTestingAudioProcessorEditor 
(TimeDomainTestingAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(400, 300);

    addAndMakeVisible(cutoffFrequencySlider);
    cutoffFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    cutoffFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    cutoffFrequencyAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(
            apvts, "frequency", cutoffFrequencySlider));

    addAndMakeVisible(cutoffFrequencyLabel);
    cutoffFrequencyLabel.setText("Cutoff Frequency", juce::dontSendNotification);
}

TimeDomainTestingAudioProcessorEditor::~TimeDomainTestingAudioProcessorEditor()
{
}

//==============================================================================
void TimeDomainTestingAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void TimeDomainTestingAudioProcessorEditor::resized()
{
    cutoffFrequencySlider.setBounds(getLocalBounds());
}
