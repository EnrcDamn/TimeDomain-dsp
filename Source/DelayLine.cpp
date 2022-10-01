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
    delayTime = static_cast<int>(dTimeMs * sampleRate) / 1000;
    if (delayTime <= 0) delayTime = 1;
    delayBuffer = new float[delayTime] {0.0f};
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

float DelayLine::readPos()
{
    out = delayBuffer[readPosition];
    return out;
}