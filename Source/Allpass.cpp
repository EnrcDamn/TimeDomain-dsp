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

float AllPass::process(float currentSample)
{
    // TODO: fix function, put buffer loop outside and add return value (sample)
    const float inputSample = currentSample;
    delayOut = delay.readPos(); // read current position (first time from empty delay buffer)
    delayIn = inputSample + (delayOut * allPassGain);
    delay.writeSample(&delayIn); // write signals sum to buffer, then update read pos
    float allPassOut = (delayIn * -allPassGain) + delayOut;

    return allPassOut; // copy allpass output to buffer
}