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

    void initDelayLine(float dTimeMs, float sampleRate);

    void prepareToPlay(float dTimeMs, float gain, float sampleRate, int mode);

    void process(juce::AudioBuffer<float>&);

    void feedforwardCombOut(juce::AudioBuffer<float>&);

    void feedbackCombOut(juce::AudioBuffer<float>&);

    void LPFCombOut(juce::AudioBuffer<float>&);

private:
    int combMode;
    float combGain;
    float delayTime;

    float delayIn;
    DelayLine delay;
    float delayOut;

    const float feedforwardGain = 1.0f; //https://ccrma.stanford.edu/~jos/pasp/Feedforward_Comb_Filters.html
};