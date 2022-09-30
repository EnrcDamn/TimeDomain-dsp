/*
  ==============================================================================

    SchroederReverb.cpp
    Created: 8 Sep 2022 3:38:00pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#include "SchroederReverb.h"

SchroederReverb::SchroederReverb() 
{

}

SchroederReverb::~SchroederReverb()
{
    delete[] allpasses;
    delete[] combs;
}

void SchroederReverb::prepareToPlay(float sampleRate)
{
    allpasses[0].prepareToPlay(9.0f, 0.7, sampleRate);
    allpasses[1].prepareToPlay(3.0f, 0.7, sampleRate);
    allpasses[2].prepareToPlay(1.0f, 0.7, sampleRate);

    combs[0].prepareToPlay(80.0f, 0.77, sampleRate, 1);
    combs[1].prepareToPlay(80.0f, 0.8, sampleRate, 1);
    combs[2].prepareToPlay(95.0f, 0.75, sampleRate, 1);
    combs[3].prepareToPlay(100.0f, 0.73, sampleRate, 1);
}

void SchroederReverb::process(juce::AudioBuffer<float>& buffer)
{
    // add the right (1) to the left (0)
    buffer.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
    // then copy the combined left (0) to the right (1)
    buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());

    // Left channel (mono input)
    auto writeSignalL = buffer.getWritePointer(0);
    // Right channel to be filled with samples after processing -> stereo out
    auto writeSignalR = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float inputSample = writeSignalL[sample];

        // 1: Allpasses in series
        for (int ap = 0; ap < apSize; ap++)
        {
            inputSample = allpasses[ap].process(inputSample);
        }

        // 2: Combs in parallel
        float combSection[4];
        for (int c = 0; c < cSize; c++)
        {
            combSection[c] = combs[c].process(inputSample);
        }

        // 3: Matrix mixing
        auto mixed = mm.mix(combSection);

        writeSignalL[sample] = mixed.first;
        writeSignalR[sample] = mixed.second;
    }
}