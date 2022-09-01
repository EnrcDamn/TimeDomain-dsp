/*
  ==============================================================================

    Comb.cpp
    Created: 1 Sep 2022 4:56:38pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#include "Comb.h"

//TODO: Low Pass Comb Filter: https://dsp.stackexchange.com/questions/60774/understanding-a-lowpass-comb-filter-implementation

Comb::Comb()
{

}

Comb::~Comb()
{

}

void Comb::initDelayLine(float dTimeMs, float sampleRate)
{
    delay.prepareToPlay(dTimeMs, sampleRate);
}

void Comb::prepareToPlay(float dTimeMs, float gain, float sampleRate)
{
    combGain = gain;
    initDelayLine(dTimeMs, sampleRate);
}

void Comb::feedforwardCombOut(juce::AudioBuffer<float>& buffer)
{

}

void Comb::feedbackCombOut(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writeSignal = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float inputSample = writeSignal[sample];
            delayOut = delay.delayRead() * -combGain;
            delayIn = (inputSample * combGain) + delayOut;
            delay.writeSample(&delayIn); // write sample to buffer, then update read pos
            const float feedbackCombOut = delayIn;

            writeSignal[sample] = feedbackCombOut; // copy allpass output to buffer
        }
    }
}

void Comb::LPFCombOut(juce::AudioBuffer<float>& buffer)
{

}