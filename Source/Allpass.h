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

    void prepareToPlay(float dTimeMs, float gain, float sampleRate);

    void setGainCoefficient(float breakFrequency);

    float getGainCoefficient();

    float process(float);

private:
    void initDelayLine(float dTimeMs, float sampleRate);

    float sampleRate;
    float delayTime;
    float gain;

    float delayIn;
    DelayLine delay;
    float delayOut;
};