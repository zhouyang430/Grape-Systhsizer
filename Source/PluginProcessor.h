/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MySynth.h"


//==============================================================================

class TestSynthesiserAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    TestSynthesiserAudioProcessor();
    ~TestSynthesiserAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    // value tree for parameters, presets, etc
    juce::AudioProcessorValueTreeState apvts;
    // juce synth
    juce::Synthesiser synth;
    //polysonic sound, 8 notes at the same time
    int voiceCount = 8;
    
    //Juce reverb
    juce::Reverb reverb;
    // Parameters of the reverb
    juce::Reverb::Parameters reverbParams;
    // Juce DSP Chorus
    juce::dsp::Chorus<float> chorus;
    //file manager
    juce::AudioFormatManager formatManager;
    //reader
    std::unique_ptr<juce::AudioFormatReader> reader;
    
    
    juce::AudioSampleBuffer fileBuffer;
    juce::AudioSampleBuffer fileBuffer1;
    juce::AudioSampleBuffer fileBuffer2;
    juce::AudioSampleBuffer fileBuffer3;
    juce::AudioSampleBuffer fileBuffer4;
    juce::AudioSampleBuffer fileBuffer5;
    juce::AudioSampleBuffer fileBuffer6;
    juce::AudioSampleBuffer fileBuffer7;
    juce::AudioSampleBuffer fileBuffer8;
    juce::AudioSampleBuffer fileBuffer9;
    juce::AudioSampleBuffer fileBuffer10;
    juce::AudioSampleBuffer fileBuffer11;
    juce::AudioSampleBuffer fileBuffer12;
    juce::AudioSampleBuffer fileBuffer13;
    juce::AudioSampleBuffer fileBuffer14;
    juce::AudioSampleBuffer fileBuffer15;
    juce::AudioSampleBuffer fileBuffer16;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestSynthesiserAudioProcessor)
};
