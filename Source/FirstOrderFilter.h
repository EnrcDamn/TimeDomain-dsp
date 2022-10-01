/*
  ==============================================================================

    FirstOrderFilter.h
    Created: 30 Sep 2022 6:36:07pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AllPass.h"

class FirstOrderFilter
{
public:
    virtual void prepareToPlay(float sampleRate);

    void setCutoffFrequency(float cutoffFrequency);

    void process(juce::AudioBuffer<float>&);

private:
    float cutoffFrequency;
    float sampleRate;
    AllPass allpass;

protected:
    float sign;
};


class LowPass : public FirstOrderFilter
{
public:
    void prepareToPlay(float sampleRate) override
    {
        setSign();
        FirstOrderFilter::prepareToPlay(sampleRate);
    }
private:
    void setSign() { this->sign = 1.0f; }
};


class HighPass : public FirstOrderFilter
{
public:
    void prepareToPlay(float sampleRate) override
    {
        setSign();
        FirstOrderFilter::prepareToPlay(sampleRate);
    }
private:
    void setSign() { this->sign = -1.0f; }
};