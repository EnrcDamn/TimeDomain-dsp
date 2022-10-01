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
    ,
    treeState(*this,
        nullptr,
        juce::Identifier("Filter"),
        {
        std::make_unique<juce::AudioParameterFloat>("frequency",
                                                    "Frequency",
                                                    juce::NormalisableRange<float>(20.f, 20000.f, 0.1f, 0.2f),
                                                    500.f)
        }
    )
#endif

{
    cutoffFrequencyParam = treeState.getRawParameterValue("frequency");
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
    lpf.prepareToPlay(static_cast<float>(sampleRate));
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

    const auto cutoffFrequency = cutoffFrequencyParam->load();

    lpf.setCutoffFrequency(cutoffFrequency);

    lpf.process(buffer);
}

//==============================================================================
bool TimeDomainTestingAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TimeDomainTestingAudioProcessor::createEditor()
{
    return new TimeDomainTestingAudioProcessorEditor (*this, treeState);
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