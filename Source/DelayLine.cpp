/*
  ==============================================================================

    DelayLine.cpp
    Created: 20 Aug 2022 11:09:11am
    Author:  Enrico Damiani

  ==============================================================================
*/
#include "DelayLine.h"


DelayLine::DelayLine()
{
    
}

DelayLine::~DelayLine()
{
    releaseResources();
}

void DelayLine::prepareToPlay(float dTimeMs, float sampleRate)
{
    delayTime = (int)(dTimeMs * sampleRate) / 1000;
    delayBuffer = new float[delayTime] {0.0f};

    for (int i = 0; i < delayTime; i++)
        DBG("Element " << juce::String(i) <<": " << juce::String(delayBuffer[i]));
}

void DelayLine::releaseResources()
{
    delete[] delayBuffer;
}

void DelayLine::writeSample(const float* currentSample)
{
    delayBuffer[readPosition] = *currentSample;
    readPosition = (readPosition != delayTime - 1 ? readPosition + 1 : 0);
}

float DelayLine::delayRead()
{
    out = delayBuffer[readPosition];
    return out;
}