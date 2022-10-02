/*
  ==============================================================================

    Comb.h
    Created: 1 Sep 2022 4:56:38pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once
#include "DelayLine.h"

class Comb
{
public:
    Comb();
    ~Comb();

    void initDelayLine(float dTimeMs, float sampleRate, int totalNumInputChannels);

    void prepareToPlay(float dTimeMs, float gain, float sampleRate, int totalNumInputChannels, int mode);

    float process(float currentSample, const int channel);

    float feedforwardCombOut(float currentSample, const int channel);

    float feedbackCombOut(float currentSample, const int channel);

    float LPFCombOut(float currentSample, const int channel); 

private:
    int mode;
    float gain;
    float delayTime;

    float delayIn;
    DelayLine delay;
    float delayOut;

    const float feedforwardGain = 1.0f; //https://ccrma.stanford.edu/~jos/pasp/Feedforward_Comb_Filters.html
};