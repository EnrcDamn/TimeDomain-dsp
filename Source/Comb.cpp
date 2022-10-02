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

void Comb::initDelayLine(float dTimeMs, float sampleRate, int totalNumInputChannels)
{
    delay.prepareToPlay(dTimeMs, sampleRate, totalNumInputChannels);
}

void Comb::prepareToPlay(float dTimeMs, float gain, float sampleRate, int totalNumInputChannels, int mode = 1)
{
    this->gain = gain;
    initDelayLine(dTimeMs, sampleRate, totalNumInputChannels);
    this->mode = mode;
}

float Comb::process(float currentSample, const int channel)
{
    jassert(mode == 0 || mode == 1 || mode == 2);
    if (mode == 0)
        return feedforwardCombOut(currentSample, channel);
    else if (mode == 1)
        return feedbackCombOut(currentSample, channel);
    else if (mode == 2)
        return LPFCombOut(currentSample, channel);

    //TODO: review this function
    return currentSample;
}

// FEEDFORWARD
float Comb::feedforwardCombOut(float currentSample, const int channel)
{
    const float inputSample = currentSample;
    delayIn = inputSample;
    delayOut = delay.readPos(channel) * gain;
    delay.writeSample(&delayIn, channel); // write sample to buffer, then update read pos
    float feedforwardCombOut = (delayIn * feedforwardGain) + delayOut;

    return feedforwardCombOut; // copy allpass output to buffer

}

// FEEDBACK
float Comb::feedbackCombOut(float currentSample, const int channel)
{
    const float inputSample = currentSample;
    delayOut = delay.readPos(channel) * -gain;
    delayIn = (inputSample * gain) + delayOut;
    delay.writeSample(&delayIn, channel); // write sample to buffer, then update read pos
    float feedbackCombOut = delayIn;

    return feedbackCombOut; // copy allpass output to buffer

}

//TODO: implement Low Pass Comb Filter: https://dsp.stackexchange.com/questions/60774/understanding-a-lowpass-comb-filter-implementation
// LPF
float Comb::LPFCombOut(float currentSample, const int channel)
{
    return currentSample;
}