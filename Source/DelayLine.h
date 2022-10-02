/*
  ==============================================================================

    DelayLine.h
    Created: 20 Aug 2022 11:09:11am
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DelayLine
{
public:
    DelayLine();
    ~DelayLine();

    void prepareToPlay(float dTimeMs, float sampleRate, int totalNumInputChannels);

    void releaseResources();

    void writeSample(const float *currentSample, const int channel);

    float readPos(const int channel);

private:
    int totalNumInputChannels;
    float** delayBuffer = nullptr;
    int readPosition = 0;
    int delayTime;
    float out;
};