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
    for (int i = 0; i < apSize; ++i)
        allpasses[i].prepareToPlay(5.0f, 0.4, static_cast<float>(sampleRate));

    
    combs[0].prepareToPlay(100.0f, 0.75, static_cast<float>(sampleRate), 1);
    combs[1].prepareToPlay(220.0f, 0.74, static_cast<float>(sampleRate), 1);
    combs[2].prepareToPlay(134.0f, 0.8, static_cast<float>(sampleRate), 1);
    combs[3].prepareToPlay(162.0f, 0.7, static_cast<float>(sampleRate), 1);
    /*ap.prepareToPlay(200.0f, 0.5, static_cast<float>(sampleRate));
    c.prepareToPlay(100.0f, 0.5, static_cast<float>(sampleRate), 1);*/
}

void TimeDomainTestingAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    delete[] allpasses;
    delete[] combs;
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

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // SCHROEDER REV
    /*for (int i = 0; i < apSize; ++i)
        allpasses[i].process(buffer);

    for (int i = 0; i < cSize; ++i)
        combs[i].process(buffer);

    mm.mix(&combs);*/


    processEffectChain(buffer);
}

// Dumb processing function for effect chain
void TimeDomainTestingAudioProcessor::processEffectChain(juce::AudioBuffer<float>& buffer)
{
    float** writeChannelSignal = new float*[buffer.getNumChannels()];
    for (int i = 0; i < buffer.getNumChannels(); ++i)
        writeChannelSignal[i] = buffer.getWritePointer(i);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float inputSample = 0.0f;
        //auto inputSample = writeSignal[sample];
        for (int i = 0; i < buffer.getNumChannels(); i++)
            inputSample += writeChannelSignal[i][sample];

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

        writeChannelSignal[0][sample] = mixed.first;
        writeChannelSignal[1][sample] = mixed.second;
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