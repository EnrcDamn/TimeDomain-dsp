/*
  ==============================================================================

    FirstOrderFilter.cpp
    Created: 30 Sep 2022 6:36:07pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#include "FirstOrderFilter.h"

// First Order LP - HP filter from AllPass: 
// https://thewolfsound.com/allpass-based-lowpass-and-highpass-filters/

void FirstOrderFilter::prepareToPlay(float sampleRate)
{
    this->sampleRate = sampleRate;
    allpass.setGainCoefficient(cutoffFrequency);
    allpass.prepareToPlay(0.f, 
                          allpass.getGainCoefficient(), 
                          sampleRate
                         );
}

void FirstOrderFilter::setCutoffFrequency(float cutoffFrequency)
{
    this->cutoffFrequency = cutoffFrequency;
    allpass.setGainCoefficient(cutoffFrequency);
}

void FirstOrderFilter::process(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writeSignal = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto inputSample = writeSignal[sample];
            auto allpassFilteredSample = allpass.process(inputSample);
            
            auto filterOutput = 0.5f * (inputSample + allpassFilteredSample * sign);

            writeSignal[sample] = filterOutput;
        }
    }
}