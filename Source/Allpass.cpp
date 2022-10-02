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

void AllPass::initDelayLine(float dTimeMs, float sampleRate, int totalNumInputChannels)
{
    delay.prepareToPlay(dTimeMs, sampleRate, totalNumInputChannels);
}

void AllPass::prepareToPlay(float dTimeMs, float gain, float sampleRate, int totalNumInputChannels)
{
    this->gain = gain;
    this->sampleRate = sampleRate;
    initDelayLine(dTimeMs, sampleRate, totalNumInputChannels);
}

void AllPass::setGainCoefficient(float breakFrequency)
{
    // Reference: https://thewolfsound.com/allpass-filter/

    float PI = juce::MathConstants<float>::pi;

    auto tan = std::tan(PI * breakFrequency / sampleRate);
    gain = (tan - 1.f) / (tan + 1.f);
}

float AllPass::getGainCoefficient()
{ 
    return gain;
}

float AllPass::process(float currentSample, const int channel)
{
    // TODO: fix function, put buffer loop outside and add return value (sample)
    const float inputSample = currentSample;
    delayOut = delay.readPos(channel); // read current position (first time from empty delay buffer)
    delayIn = inputSample + (delayOut * -gain);
    delay.writeSample(&delayIn, channel); // write signals sum to buffer, then update read pos
    float allPassOut = (delayIn * gain) + delayOut;

    return allPassOut; // copy allpass output to buffer
}