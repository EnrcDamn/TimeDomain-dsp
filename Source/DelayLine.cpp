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

    /*readPosition = 0;
    delayTime;
    out;*/
}

DelayLine::~DelayLine()
{
    releaseResources();
}

void DelayLine::prepareToPlay(float dTimeMs, float sampleRate, int totalNumInputChannels)
{
    delayTime = static_cast<int>(dTimeMs * sampleRate) / 1000;
    if (delayTime <= 0) delayTime = 1;
    this->totalNumInputChannels = totalNumInputChannels;

    if (delayBuffer != nullptr) releaseResources();

    delayBuffer = new float* [totalNumInputChannels];
    for (int i = 0; i < totalNumInputChannels; i++)
        delayBuffer[i] = new float [delayTime] {0.f};
}

void DelayLine::releaseResources()
{
    for (int i = 0; i < totalNumInputChannels; i++)
        delete[] delayBuffer[i];
    delete[] delayBuffer;
}

void DelayLine::writeSample(const float* currentSample, const int channel)
{
    delayBuffer[channel][readPosition] = *currentSample;
    readPosition = (readPosition != delayTime - 1 ? readPosition + 1 : 0);
}

float DelayLine::readPos(const int channel)
{
    out = delayBuffer[channel][readPosition];
    return out;
}