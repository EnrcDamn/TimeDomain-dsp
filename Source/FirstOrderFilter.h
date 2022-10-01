/*
  ==============================================================================

    HighPass.h
    Created: 30 Sep 2022 6:36:07pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AllPass.h"
#include <vector>

class HighPass
{
public:
    void prepareToPlay(float sampleRate);
    void setCutoffFrequency(float cutoffFrequency);
    void process(juce::AudioBuffer<float>&);

private:
    float cutoffFrequency;
    float sampleRate;
    AllPass allpass;
};