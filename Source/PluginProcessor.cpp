/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TimeDomainTestingAudioProcessor::TimeDomainTestingAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

TimeDomainTestingAudioProcessor::~TimeDomainTestingAudioProcessor()
{
    delete[] allpasses;
    delete[] combs;
}

//==============================================================================
const juce::String TimeDomainTestingAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TimeDomainTestingAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TimeDomainTestingAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TimeDomainTestingAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TimeDomainTestingAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TimeDomainTestingAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TimeDomainTestingAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TimeDomainTestingAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TimeDomainTestingAudioProcessor::getProgramName (int index)
{
    return {};
}

void TimeDomainTestingAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TimeDomainTestingAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    allpasses[0].prepareToPlay(9.0f, 0.7, static_cast<float>(sampleRate));
    allpasses[1].prepareToPlay(3.0f, 0.7, static_cast<float>(sampleRate));
    allpasses[2].prepareToPlay(1.0f, 0.7, static_cast<float>(sampleRate));

    
    combs[0].prepareToPlay(80.0f, 0.77, static_cast<float>(sampleRate), 1);
    combs[1].prepareToPlay(80.0f, 0.8, static_cast<float>(sampleRate), 1);
    combs[2].prepareToPlay(95.0f, 0.75, static_cast<float>(sampleRate), 1);
    combs[3].prepareToPlay(100.0f, 0.73, static_cast<float>(sampleRate), 1);

    ap.prepareToPlay(200.0f, 0.5, static_cast<float>(sampleRate));
    c.prepareToPlay(100.0f, 0.5, static_cast<float>(sampleRate), 1);
}

void TimeDomainTestingAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TimeDomainTestingAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TimeDomainTestingAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    processEffectChain(buffer);
    
}

// Dumb processing function for effect chain
void TimeDomainTestingAudioProcessor::processEffectChain(juce::AudioBuffer<float>& buffer)
{
    
    // add the right (1) to the left (0)
    buffer.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
    // then copy the combined left (0) to the right (1)
    buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());

    auto writeSignalMono = buffer.getWritePointer(0);
    auto writeSignalStereo = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        //DBG("Input L: " << juce::String(writeSignalMono[sample]));
        //DBG("Input R: " << juce::String(writeSignalStereo[sample]));
        float inputSample = writeSignalMono[sample];

        for (int ap = 0; ap < apSize; ap++)
        {
            inputSample = allpasses[ap].process(inputSample);
        }

        float combSection[4];
        for (int c = 0; c < cSize; c++)
        {
            combSection[c] = combs[c].process(inputSample);
        }

        auto mixed = mm.mix(combSection);

        writeSignalMono[sample] = mixed.first;
        writeSignalStereo[sample] = mixed.second;

        //DBG("Output L: " << juce::String(writeSignalMono[sample]));
        //DBG("Output R: " << juce::String(writeSignalStereo[sample]));
    }
}

void TimeDomainTestingAudioProcessor::processAP(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto writeSignal = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = writeSignal[sample];

            auto out = ap.process(inputSample);
            writeSignal[sample] = out;
        }
    }
}

//==============================================================================
bool TimeDomainTestingAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TimeDomainTestingAudioProcessor::createEditor()
{
    return new TimeDomainTestingAudioProcessorEditor (*this);
}

//==============================================================================
void TimeDomainTestingAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TimeDomainTestingAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TimeDomainTestingAudioProcessor();
}