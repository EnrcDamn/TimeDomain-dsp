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

float Comb::process(float currentSample)
{
    jassert(combMode == 0 || combMode == 1 || combMode == 2);
    if (combMode == 0)
        return feedforwardCombOut(currentSample);
    else if (combMode == 1)
        return feedbackCombOut(currentSample);
    else if (combMode == 2)
        return LPFCombOut(currentSample);

    //TODO: review this function
    else
        return currentSample;
}

// FEEDFORWARD
float Comb::feedforwardCombOut(float currentSample)
{
    const float inputSample = currentSample;
    delayIn = inputSample;
    delayOut = delay.readPos() * combGain;
    delay.writeSample(&delayIn); // write sample to buffer, then update read pos
    float feedforwardCombOut = (delayIn * feedforwardGain) + delayOut;

    return feedforwardCombOut; // copy allpass output to buffer

}

// FEEDBACK
float Comb::feedbackCombOut(float currentSample)
{
    const float inputSample = currentSample;
    delayOut = delay.readPos() * -combGain;
    delayIn = (inputSample * combGain) + delayOut;
    delay.writeSample(&delayIn); // write sample to buffer, then update read pos
    float feedbackCombOut = delayIn;

    return feedbackCombOut; // copy allpass output to buffer

}

//TODO: implement Low Pass Comb Filter: https://dsp.stackexchange.com/questions/60774/understanding-a-lowpass-comb-filter-implementation
// LPF
float Comb::LPFCombOut(float currentSample)
{
    return currentSample;
}