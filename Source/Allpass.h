/*
  ==============================================================================

    Allpass.h
    Created: 17 Aug 2022 10:54:15am
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once
#include "DelayLine.h"

class AllPass
{

public:
    AllPass();
    ~AllPass();

    void prepareToPlay(float dTimeMs, float gain, float sampleRate, int totalNumInputChannels);

    void setGainCoefficient(float breakFrequency);

    float getGainCoefficient();

    float process(float currentSample, const int channel);

private:
    void initDelayLine(float dTimeMs, float sampleRate, int totalNumInputChannels);

    float sampleRate;
    float delayTime;
    float gain;

    float delayIn;
    float delayOut;
    DelayLine delay;
};