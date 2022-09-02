/*
  ==============================================================================

    Allpass.cpp
    Created: 17 Aug 2022 10:54:15am
    Author:  Enrico Damiani

  ==============================================================================
*/

#include "Allpass.h"

AllPass::AllPass()
{

}

AllPass::~AllPass()
{

}

void AllPass::initDelayLine(float dTimeMs, float sampleRate)
{
    delay.prepareToPlay(dTimeMs, sampleRate);
}

void AllPass::prepareToPlay(float dTimeMs, float gain, float sampleRate)
{
    allPassGain = gain;
    initDelayLine(dTimeMs, sampleRate);
}

void AllPass::process(juce::AudioBuffer<float>& buffer)
{
    // TODO: fix function, put buffer loop outside and add return value (sample)
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writeSignal = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float inputSample = writeSignal[sample];
            delayOut = delay.readPos(); // read current position (first time from empty delay buffer)
            delayIn = inputSample + (delayOut * allPassGain);
            delay.writeSample(&delayIn); // write sample to buffer, then update read pos
            const float allPassOut = (delayIn + delayOut) * -allPassGain;

            writeSignal[sample] = allPassOut; // copy allpass output to buffer
        }
    }
}