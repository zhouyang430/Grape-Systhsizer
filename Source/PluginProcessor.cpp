/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//for creating string
#include <iostream>
#include <string>

//==============================================================================
TestSynthesiserAudioProcessor::TestSynthesiserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
apvts(*this,nullptr,"ParamTree",{
    std::make_unique<juce::AudioParameterChoice>("switchWave", "Switch Wave", juce::StringArray({"Phase Moduled Sound", "Osc Mix", "Samples"}), 0),
    std::make_unique<juce::AudioParameterFloat>("phaseModFre", "Phase Mod Carrier Frequency", 0.001, 50, 5),
    std::make_unique<juce::AudioParameterFloat>("phaseModDepth", "Phase Mod Depth",1, 10, 2),
    std::make_unique<juce::AudioParameterChoice>("phaseModType", "Phase Mod Type", juce::StringArray({"SinOsc","SquareOsc","TriOsc","SawOsc"}), 0),
    std::make_unique<juce::AudioParameterChoice>("oscChoice1", "Osc 1", juce::StringArray({"SinOsc", "SquareOsc", "TriOsc", "SawOsc"}), 0),
    std::make_unique<juce::AudioParameterChoice>("oscChoice2", "Osc 2", juce::StringArray({"SinOsc", "SquareOsc", "TriOsc", "SawOsc"}), 0),
    std::make_unique<juce::AudioParameterFloat>("oscMix", "Osc Mix",0, 1, 0.5),
    std::make_unique<juce::AudioParameterChoice>("sampleType", "Sample Type", juce::StringArray({"Drum","Get your inspiration!", "Fx", "Atmosphere"}), 0),
    std::make_unique<juce::AudioParameterChoice>("drum", "Drum", juce::StringArray({"atmosphere 808", "broken 808", "awesome new kick", "snare", "hat"}), 0),
    std::make_unique<juce::AudioParameterChoice>("loop", "Get your inspiration!", juce::StringArray({"Intro Rhodes 80 F#m", "Verse Pluck 75 F#m", "Wonkiest Drums Out (100)"}), 0),
    std::make_unique<juce::AudioParameterChoice>("fx", "Fx", juce::StringArray({"Trian Coming", "Robot Impact", "Samples-Scary Motherboard", "Lazer Downlifter"}), 0),
    std::make_unique<juce::AudioParameterChoice>("atmos", "Atmosphere", juce::StringArray({"atmos-54", "atmos-06", "Crowd Cheering", "Forest Ambience"}), 0),
    std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0, 0.5, 0.15),
    std::make_unique<juce::AudioParameterFloat>("attack","Attack Time",0.01, 3, 0.01),
    std::make_unique<juce::AudioParameterFloat>("decay", "Decay Time", 0.01, 3,1),
    std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain Time", 0.01, 3, 1),
    std::make_unique<juce::AudioParameterFloat>("release", "Release Time", 0.01, 3, 0.01),
    std::make_unique<juce::AudioParameterFloat>("noiseMix", "Noise Mix", 0, 1, 0),
    std::make_unique<juce::AudioParameterChoice>("filtertype", "Filter Type", juce::StringArray({"All pass", "Low pass", "High pass", "Notch", "Band pass"}), 0),
    std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 500, 20000, 10000),
    std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", 0.1, 10, 1),
    std::make_unique<juce::AudioParameterChoice>("noLFO","Use LFO in filter cutoff?",juce::StringArray({"No", "Yes"}), 0),
    std::make_unique<juce::AudioParameterChoice>("lfo_shape", "LFO shape",juce::StringArray({"SinOsc", "TriOsc", "SawOsc", "SquareOsc"}), 0),
    std::make_unique<juce::AudioParameterFloat>("lfo_freq", "LFO Freq", 0, 100, 10),
    std::make_unique<juce::AudioParameterFloat>("lfoFrePan","Auto Pan LFO Frequency", 0, 0.2, 0),
    std::make_unique<juce::AudioParameterFloat>("chorus_depth", "Chorus Depth", 0, 1, 0.1),
    std::make_unique<juce::AudioParameterFloat>("chorus_feedback", "Chorus Feedback", -0.98, 0.98, 0),
    std::make_unique<juce::AudioParameterFloat>("chorus_center_delay", "Chorus Center Delay", 1, 100, 10),
    std::make_unique<juce::AudioParameterFloat>("chorus_mix", "Chorus Mix", 0, 1, 0.1),
    std::make_unique<juce::AudioParameterFloat>("room_size", "Room size",0, 1, 0.1),
    std::make_unique<juce::AudioParameterFloat>("damping", "Damping", 0, 1, 0.1),
    std::make_unique<juce::AudioParameterFloat>("dry", "Dry",0, 1, 0.3),
    std::make_unique<juce::AudioParameterFloat>("wet", "Wet",0, 1, 0.3)
    
    })

{   //move to another easier method to set the value-->
    //attackParam = apvts.getRawParameterValue("attack");
    
    for(int voiceNum = 0; voiceNum < voiceCount; voiceNum++)
    {
        //DBG(apvts.getRawParameterValue("usesample"));
        synth.addVoice(new MySynthVoice());
        formatManager.registerBasicFormats();
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::atmosphere_808_C_wav,BinaryData::atmosphere_808_C_wavSize,false)));
        fileBuffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::broken_808_F_wav,BinaryData::broken_808_F_wavSize,false)));
        fileBuffer1.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer1,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::awesome_new_kick_wav,BinaryData::awesome_new_kick_wavSize,false)));
        fileBuffer2.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer2,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Snare_45_wav,BinaryData::Snare_45_wavSize,false)));
        fileBuffer3.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer3,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Hat_2_wav,BinaryData::Hat_2_wavSize,false)));
        fileBuffer4.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer4,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Open_Hat_4_wav,BinaryData::Open_Hat_4_wavSize,false)));
        fileBuffer5.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer5,0,(int)reader->lengthInSamples,0,true,true);
        
        // read loop audios in buffer Intro_Rhodes_80_Fm_wav
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Intro_Rhodes_80_Fm_wav,BinaryData::Intro_Rhodes_80_Fm_wavSize,false)));
        fileBuffer6.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer6,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Verse_Pluck_75_Fm_wav,BinaryData::Verse_Pluck_75_Fm_wavSize,false)));
        fileBuffer7.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer7,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Wonkiest_Drums_Out_100_wav,BinaryData::Wonkiest_Drums_Out_100_wavSize,false)));
        fileBuffer8.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer8,0,(int)reader->lengthInSamples,0,true,true);
        //read the fx audios in buffer
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Train_Coming_wav,BinaryData::Train_Coming_wavSize,false)));
        fileBuffer9.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer9,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Robot_Impact_wav,BinaryData::Robot_Impact_wavSize,false)));
        fileBuffer10.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer10,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Scary_Motherboard_wav,BinaryData::Scary_Motherboard_wavSize,false)));
        fileBuffer11.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer11,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Lazer_Downlifter_wav,BinaryData::Lazer_Downlifter_wavSize,false)));
        fileBuffer12.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer12,0,(int)reader->lengthInSamples,0,true,true);
        //read atoms
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::atmos54_wav,BinaryData::atmos54_wavSize,false)));
        fileBuffer13.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer13,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::atmos06_wav,BinaryData::atmos06_wavSize,false)));
        fileBuffer14.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer14,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Crowd_Cheering__5_wav,BinaryData::Crowd_Cheering__5_wavSize,false)));
        fileBuffer15.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer15,0,(int)reader->lengthInSamples,0,true,true);
        
        reader.reset( formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::Forest_Ambience_wav,BinaryData::Forest_Ambience_wavSize,false)));
        fileBuffer16.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer16,0,(int)reader->lengthInSamples,0,true,true);
          
        formatManager.clearFormats();
       
    }
    // control the keyboard
    synth.addSound(new MySynthSound());
    
    
}


void TestSynthesiserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //check if wave table is enabled
    
    synth.setCurrentPlaybackSampleRate(sampleRate);
   
    for (int i=0; i<voiceCount; i++)
    {
        
        MySynthVoice * v =dynamic_cast<MySynthVoice*>(synth.getVoice(i));
        v->init(sampleRate);
        v->readInAudio(fileBuffer,fileBuffer1,fileBuffer2,fileBuffer3,fileBuffer4,fileBuffer5,fileBuffer6,fileBuffer7,fileBuffer8,fileBuffer9,fileBuffer10,fileBuffer11,fileBuffer12,fileBuffer13,fileBuffer14,fileBuffer15,fileBuffer16);
    }
    // set spec for using chorus and reverb
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;//getNumOutputChannels();
    chorus.prepare(spec);
    chorus.reset();
    // Preparing the reverb with a reset
    reverb.reset();
    
}


void TestSynthesiserAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    srand( static_cast<unsigned int>(time(nullptr)));
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    for(int voiceNum = 0; voiceNum < voiceCount; voiceNum++)
    {
        MySynthVoice* voicePtr = dynamic_cast<MySynthVoice*>(synth.getVoice(voiceNum));
        //the operation is object not the ptr
        voicePtr->updateGain(apvts.getRawParameterValue("gain"));
        voicePtr->connectEnvelopeParameters(apvts.getRawParameterValue("attack"),apvts.getRawParameterValue("decay"),apvts.getRawParameterValue("sustain"),apvts.getRawParameterValue("release"));
        
        voicePtr->updatePhaseModParam(apvts.getRawParameterValue("phaseModFre"),apvts.getRawParameterValue("phaseModDepth"),apvts.getRawParameterValue("phaseModType"));
        voicePtr->updateOscChoice1(apvts.getRawParameterValue("oscChoice1"));
        voicePtr->updateOscChoice2(apvts.getRawParameterValue("oscChoice2"));
        voicePtr->updateOscMix(apvts.getRawParameterValue("oscMix"));
        voicePtr->updateNoiseMix(apvts.getRawParameterValue("noiseMix"));
        voicePtr->updateWaveMode(apvts.getRawParameterValue("switchWave"));
        voicePtr->updateFilterParam(apvts.getRawParameterValue("cutoff"),apvts.getRawParameterValue("resonance"),apvts.getRawParameterValue("filtertype"),apvts.getRawParameterValue("noLFO"),apvts.getRawParameterValue("lfo_freq"), apvts.getRawParameterValue("lfo_shape"));
        voicePtr->updateLFOforPan(apvts.getRawParameterValue("lfoFrePan"));
        voicePtr->updateSampleType(apvts.getRawParameterValue("sampleType"));
        voicePtr->updateAudio(apvts.getRawParameterValue("drum"),apvts.getRawParameterValue("loop"),apvts.getRawParameterValue("fx"),apvts.getRawParameterValue("atmos"));
            
    }
  
    chorus.setDepth(*apvts.getRawParameterValue("chorus_depth"));
    chorus.setMix(*apvts.getRawParameterValue("chorus_mix"));
    chorus.setCentreDelay(*apvts.getRawParameterValue("chorus_center_delay"));
    chorus.setFeedback(*apvts.getRawParameterValue("chorus_feedback"));

    juce::dsp::AudioBlock<float> sampleBlock(buffer);
    chorus.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));

    reverbParams.roomSize = *apvts.getRawParameterValue("room_size");
    reverbParams.damping = *apvts.getRawParameterValue("damping");
    reverbParams.dryLevel = *apvts.getRawParameterValue("dry");
    reverbParams.wetLevel = *apvts.getRawParameterValue("wet");
    reverb.setParameters(reverbParams);
    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
   

}


TestSynthesiserAudioProcessor::~TestSynthesiserAudioProcessor()
{
}

//==============================================================================
const juce::String TestSynthesiserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestSynthesiserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestSynthesiserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestSynthesiserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestSynthesiserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestSynthesiserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestSynthesiserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestSynthesiserAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TestSynthesiserAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestSynthesiserAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

void TestSynthesiserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestSynthesiserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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



//==============================================================================
bool TestSynthesiserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TestSynthesiserAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void TestSynthesiserAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TestSynthesiserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (apvts.state.getType()))
        {
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestSynthesiserAudioProcessor();
}
