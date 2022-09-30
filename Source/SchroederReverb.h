/*
  ==============================================================================

    SchroederReverb.h
    Created: 8 Sep 2022 3:38:00pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once
#include "Allpass.h"
#include "Comb.h"
#include "MixMatrix.h"


class SchroederReverb
{
public:
    SchroederReverb();
    ~SchroederReverb();

    void prepareToPlay(float sampleRate);
    void process(juce::AudioBuffer<float>&);

private:
    int apSize = 3;
    AllPass* allpasses = new AllPass[apSize];
    int cSize = 4;
    Comb* combs = new Comb[cSize];
    SimpleMixMatrix mm;

};