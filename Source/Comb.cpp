/*
  ==============================================================================

    Comb.cpp
    Created: 1 Sep 2022 4:56:38pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#include "Comb.h"

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

void Comb::prepareToPlay(float dTimeMs, float gain, float sampleRate, int mode)
{
    combGain = gain;
    initDelayLine(dTimeMs, sampleRate);
    combMode = mode;
}

void Comb::process(juce::AudioBuffer<float>& buffer)
{
    jassert(combMode == 0 || combMode == 1 || combMode == 2);
    if (combMode == 0)
        feedforwardCombOut(buffer);
    else if (combMode == 1)
        feedbackCombOut(buffer);
    else if (combMode == 2)
        LPFCombOut(buffer);
}

// FEEDFORWARD
void Comb::feedforwardCombOut(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writeSignal = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float inputSample = writeSignal[sample];
            delayIn = inputSample;
            delayOut = delay.readPos() * combGain;
            delay.writeSample(&delayIn); // write sample to buffer, then update read pos
            const float feedforwardCombOut = (delayIn * feedforwardGain) + delayOut;

            writeSignal[sample] = feedforwardCombOut; // copy allpass output to buffer
        }
    }
}

// FEEDBACK
void Comb::feedbackCombOut(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writeSignal = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float inputSample = writeSignal[sample];
            delayOut = delay.readPos() * -combGain;
            delayIn = (inputSample * combGain) + delayOut;
            delay.writeSample(&delayIn); // write sample to buffer, then update read pos
            const float feedbackCombOut = delayIn;

            writeSignal[sample] = feedbackCombOut; // copy allpass output to buffer
        }
    }
}

//TODO: Low Pass Comb Filter: https://dsp.stackexchange.com/questions/60774/understanding-a-lowpass-comb-filter-implementation
// LPF
void Comb::LPFCombOut(juce::AudioBuffer<float>& buffer)
{

}