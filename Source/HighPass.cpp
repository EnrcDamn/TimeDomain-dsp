/*
  ==============================================================================

    HighPass.cpp
    Created: 30 Sep 2022 6:36:07pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#include "HighPass.h"

void HighPass::prepareToPlay(float sampleRate)
{
    this->sampleRate = sampleRate;
    allpass.prepareToPlay(0.0f, 1.0, sampleRate);
}

void HighPass::setCutoffFrequency(float cutoffFrequency)
{
    this->cutoffFrequency = cutoffFrequency;
}

void HighPass::process(juce::AudioBuffer<float>& buffer)
{
    float PI = juce::MathConstants<float>::pi;

    auto tan = std::tan(PI * cutoffFrequency / sampleRate);
    auto a1 = (tan - 1.f) / (tan + 1.f);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writeSignal = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto inputSample = writeSignal[sample];
            auto allpassFilteredSample = allpass.process(inputSample);
            
            auto filterOutput = 0.5 * (inputSample + allpassFilteredSample * -1.f);

            writeSignal[sample] = filterOutput;
        }
    }
}