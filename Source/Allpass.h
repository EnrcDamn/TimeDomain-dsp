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

    float process(float);

private:
    void initDelayLine(float dTimeMs, float sampleRate);

    float allPassGain;
    float delayTime;

    float delayIn;
    DelayLine delay;
    float delayOut;
};