/**
@file MySynth.h
@brief Two classes: MynthSound is used to control the sound and MySynthVoice is used to control the voice
@author B200097
@version 1.0
@date 13/12/2022
@note This is modified by Matthew's basic MySynth.h. Thanks to Matthew.
*/
#include <JuceHeader.h>
#include "Oscillators.h"
#include "PhaseModulation.h"
#include "Filters.h"

#pragma once
// SOUND
class MySynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};

// =================================
// =================================
// Synthesiser Voice - my synth code goes in here

/**
 @class MySynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple MySynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 @namespace none
 @updated 2019-06-18
 */
class MySynthVoice : public :: juce::SynthesiserVoice
{


// =================================
// =================================
// Synthesiser Voice - my synth code goes in here

public:
    /**
    @brief update sample trye
    @param _sampleType sampletype for user to choose. Drum, Inspiration clips, Fx, Atomosphere are provided
    */
    void updateSampleType(std::atomic<float>* _sampleType)
    {
        sampleType = *_sampleType;
    }
    
    /**
    @brief update audio after user choose a type of sample
    @param _drum for user to choose different drum samples
    @param _loop for user to choose different Inspiration clips
    @param _fx for user to choose different Fx
    @param _atmos for user to choose different atmosphere sounds
    */
    void updateAudio(std::atomic<float>* _drum,std::atomic<float>* _loop,std::atomic<float>* _fx,std::atomic<float>* _atmos)
    {
        drum = *_drum;
        loop = *_loop;
        fx = *_fx;
        atmos = *_atmos;
        
    }
    /**
    @brief intialize sample rate for essencial case like osc, envolope, noise, etc.
    @param sampleRate current sample rate
    */
    void init(float sampleRate)
    {
        // set sample rate for the first osc table
        osc1_0.setSampleRate(getSampleRate());
        osc1_1.setSampleRate(getSampleRate());
        osc1_2.setSampleRate(getSampleRate());
        osc1_3.setSampleRate(getSampleRate());
        
        // set sample rate for the second osc table
        osc2_0.setSampleRate(getSampleRate());
        osc2_1.setSampleRate(getSampleRate());
        osc2_2.setSampleRate(getSampleRate());
        osc2_3.setSampleRate(getSampleRate());
        
        // set sample rate for the phase modulator and carrier
        phaseMod.setSampleRate(getSampleRate());
        
        // set sample rate for noise
        //noise.setSampleRate(getSampleRate());
        
        // set sample rate for envelope
        envelope.setSampleRate(getSampleRate());
        
        // set sample rate for filter
        filter.setSampleRate(getSampleRate());
        // reset filter
        filter.resetInnerFilter();
        
        // set sample rate for pan lfo
        lfoPan.setSampleRate(getSampleRate());
    }
    /**
    @brief update Gain
    @param _gain current volumn
    */
    void updateGain(std::atomic<float>* _gain)
    {
        gain = *_gain;
    }
    
    /**
    @brief update the wave mode
    @param _waveMode the wave mode use choose
    */
    void updateWaveMode(std::atomic<float>* _waveMode)
    {
        waveMode = *_waveMode;
    }
    
    /**
    @brief intialize sample rate for essencial case like osc, envolope, noise, etc.
    @param _attackParam current attack
    @param _decayParam current decay
    @param _sustainParam current sustain
    @param _releaseParam current release
    */
    void connectEnvelopeParameters(std::atomic<float>* _attackParam,std::atomic<float>* _decayParam,std::atomic<float>* _sustainParam,std::atomic<float>* _releaseParam)
    {
        attackParam = _attackParam;
        decayParam = _decayParam;
        sustainParam = _sustainParam;
        releaseParam = _releaseParam;
        
    }
    
    /**
    @brief update choice for osc1
    @param _oscChoice1 osc choice - sin, square, tri, saw
    */
    void updateOscChoice1(std::atomic<float>* _oscChoice1)
    {
        oscChoice1 = *_oscChoice1;
    }
    /**
     @brief update choice for osc2
     @param _oscChoice2 osc choice - sin, square, tri, saw
    */
    void updateOscChoice2(std::atomic<float>* _oscChoice2)
    {
        oscChoice2 = *_oscChoice2;
    }
    
    
    /**
    @brief update mix ratio for two oscs
    @param _oscMix mix ratio for two oscs
    */
    void updateOscMix(std::atomic<float>* _oscMix)
    {
        oscMix = *_oscMix;
    }
    
    /**
    @brief update Phase Modulation Param
    @param _phaseModFre frequency for modulator
    @param _phaseModDepth times for modulation
    @param _phaseModType the shape of modulator
    */
    void updatePhaseModParam(std::atomic<float>* _phaseModFre,std::atomic<float>* _phaseModDepth,std::atomic<float>* _phaseModType)
    {
        phaseModFre = *_phaseModFre;
        phaseModDepth = *_phaseModDepth;
        phaseModType = *_phaseModType;
    }
    
    /**
     @brief update mix ratio for oscs and noise
     @param _noiseMix mix ratio for oscs and noise
    */
    void updateNoiseMix(std::atomic<float>* _noiseMix)
    {
        noiseMix = *_noiseMix;
    }
    
    /**
    @brief update Filter Param
    @param _cutoff cutoff frequency
    @param _resonance resoncance
    @param _filtertype full/low/high/notch/band pass
    @param _noLFO use lfo to control the filter cutoff frequency or not
    @param _lfoFreq frequency for lfo
    @param _lfoShape lfo shape
    */
    void  updateFilterParam(std::atomic<float>* _cutoff,std::atomic<float>* _resonance,std::atomic<float>* _filtertype,std::atomic<float>* _noLFO,std::atomic<float>* _lfoFreq, std::atomic<float>* _lfoShape)
    {
        cutoff = *_cutoff;
        resonance = *_resonance;
        filtertype = * _filtertype;
        noLFO = *_noLFO;
        lfoShape = int(*_lfoShape);
        lfoFreq = *_lfoFreq;
    }
    /**
    @brief update LFO for Pan
    @param _lfoFrePan lfo to change the pan
    */
    void  updateLFOforPan(std::atomic<float>* _lfoFrePan)
    {
        lfoFrePan = * _lfoFrePan;
    }

    //--------------------------------------------------------------------------
    /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        // set envolope parameters
        envParameters.attack = *attackParam;
        envParameters.decay = *decayParam;
        envParameters.sustain = *sustainParam;
        envParameters.release = *releaseParam;
        envelope.setParameters(envParameters);
        envelope.noteOn();
       
        //convert pitch
        freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
      
        // set frequency for 2 oscllators
        osc1_0.setFrequency(freq);
        osc1_1.setFrequency(freq);
        osc1_2.setFrequency(freq);
        osc1_3.setFrequency(freq);
        osc2_0.setFrequency(freq);
        osc2_1.setFrequency(freq);
        osc2_2.setFrequency(freq);
        osc2_3.setFrequency(freq);
        
        // set frequency for carrier
        phaseMod.setFrequencyC(freq);
        //update the modulator's frequency
        phaseMod.setFrequencyM(phaseModFre);
        
        // set frequency for lfo pan
        lfoPan.setFrequency(lfoFrePan);
        // set the velocity to control the gain
        gainVol = velocity;
        
        playing = true;
        
    }
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        envelope.noteOff();
        
    }
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put your DSP code in here
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if(playing)
        {
            float* left = outputBuffer.getWritePointer(0);
            float* right = outputBuffer.getWritePointer(1);
            
            // two buffer pointers for each channel, it has risk to use getReadPointer
            // instead, using getWritePointer would tell the compiler that this is a reusable, changable pointer
            const float* waveDataL;
            const float* waveDataR;
            
            // cases to switch from sample tables, assign the right audio file to the dropdown options
            if(int(sampleType) == 0)
            {
                if(int(drum) == 0)
                {
                    waveDataL = fileBuffer.getWritePointer(0);
                    waveDataR = fileBuffer.getWritePointer(1);
                }
                
                else if(int(drum) == 1)
                {
                    waveDataL = fileBuffer1.getWritePointer(0);
                    waveDataR = fileBuffer1.getWritePointer(1);
                    
                }
                else if(int(drum) == 2)
                {
                    waveDataL = fileBuffer2.getWritePointer(0);
                    waveDataR = fileBuffer2.getWritePointer(1);
                }
                else if(int(drum) == 3)
                {
                    waveDataL = fileBuffer3.getWritePointer(0);
                    waveDataR = fileBuffer3.getWritePointer(1);
                }
                else if(int(drum) == 4)
                {
                    waveDataL = fileBuffer4.getWritePointer(0);
                    waveDataR = fileBuffer4.getWritePointer(1);
                }
                
            }
            else if(int(sampleType) == 1)
            {
                if(int(loop) == 0)
                {
                    waveDataL = fileBuffer6.getWritePointer(0);
                    waveDataR = fileBuffer6.getWritePointer(1);
                }
                
                else if(int(loop) == 1)
                {
                    waveDataL = fileBuffer7.getWritePointer(0);
                    waveDataR = fileBuffer7.getWritePointer(1);
                }
                else if(int(loop) == 2)
                {
                    waveDataL = fileBuffer8.getWritePointer(0);
                    waveDataR = fileBuffer8.getWritePointer(1);
                }
                
            }
            else if(int(sampleType) == 2)
            {
                if(int(fx) == 0)
                {
                    waveDataL = fileBuffer9.getWritePointer(0);
                    waveDataR = fileBuffer9.getWritePointer(1);
                }
                
                else if(int(fx) == 1)
                {
                    waveDataL = fileBuffer10.getWritePointer(0);
                    waveDataR = fileBuffer10.getWritePointer(1);
                }
                else if(int(fx) == 2)
                {
                    waveDataL = fileBuffer11.getWritePointer(0);
                    waveDataR = fileBuffer11.getWritePointer(1);
                }
                else if(int(fx) == 3)
                {
                    waveDataL = fileBuffer12.getWritePointer(0);
                    waveDataR = fileBuffer12.getWritePointer(1);
                }
                
            }
            else if(int(sampleType) == 3)
            {
                if(int(atmos) == 0)
                {

                    waveDataL = fileBuffer13.getWritePointer(0);
                    waveDataR = fileBuffer13.getWritePointer(1);
                }
                
                else if(int(atmos) == 1)
                {
                    waveDataL = fileBuffer14.getWritePointer(0);
                    waveDataR = fileBuffer14.getWritePointer(1);
                }
                else if(int(atmos) == 2)
                {
                    waveDataL = fileBuffer15.getWritePointer(0);
                    waveDataR = fileBuffer15.getWritePointer(1);
                }
                else if(int(atmos) == 3)
                {
                    waveDataL = fileBuffer16.getWritePointer(0);
                    waveDataR = fileBuffer16.getWritePointer(1);
                }
                
            }
            
           
            // interate the samples
            for(int i = startSample; i < startSample+numSamples; i++)
            {
                //get nest envolope params
                envVal = envelope.getNextSample();
                // cases to swith from different shapes of oscillators
                switch(int(oscChoice1))
                {
                    case 0:
                        oscVal_1 = osc1_0.process();
                        break;
                    case 1:
                        oscVal_1 = osc1_1.process();
                        break;
                    case 2:
                        oscVal_1 = osc1_2.process();
                        break;
                    case 3:
                        oscVal_1 = osc1_3.process();
                        break;
                }
                switch(int(oscChoice2))
                {
                    case 0:
                        oscVal_2 = osc2_0.process();
                        break;
                    case 1:
                        oscVal_2 = osc2_1.process();
                        break;
                    case 2:
                        oscVal_2 = osc2_2.process();
                        break;
                    case 3:
                        oscVal_2 = osc2_3.process();
                        break;
                }
                // cases to swith from different shapes of modulators for phase modulation
                switch(int(waveMode))
                {
                    case 0:
                    {
                        noise = random.nextFloat();
                        //phase modulation is called here
                        mixOsc = phaseMod.process(1,int(phaseModDepth),int(phaseModType));
                        // mix the noise
                        mixOscNoise = (mixOsc * (1 - noiseMix) + noise * noiseMix)
                        * envVal * gainVol;
                        // filter is called here
                        outputVal = filter.process(mixOscNoise,cutoff,resonance,filtertype,lfoFreq,lfoShape,noLFO);
                        //set samples in each channel
                        left[i] +=  outputVal *  (lfoPan.process() * gain + gain);
                        right[i] += outputVal * ((-lfoPan.process() * gain + gain));
                    break;
                    }
                    case 1:
                    {
                        noise = random.nextFloat();
                        // mix two oscillators
                        mixOsc = oscMix * oscVal_1  + (1 - oscMix) * oscVal_2;
                        //mix with the noise
                        mixOscNoise = (mixOsc * (1 - noiseMix) + noise * noiseMix)
                        * envVal * gainVol;
                        // filter is called here
                        outputVal = filter.process(mixOscNoise,cutoff,resonance,filtertype,lfoFreq,lfoShape,noLFO);
                        //set samples in each channel
                        left[i] +=  outputVal *  (lfoPan.process() * gain + gain);
                        right[i] += outputVal * ((-lfoPan.process()* gain + gain));
    
                    break;
                    case 2:
                        // mutiply 4 to make good EQ from my synth samples, also apply ADSR and pan control
                        mixOscNoise = (float(waveDataL[counter]) * (1 - noiseMix) + noise
                                       * noiseMix)   * envVal * gainVol;
                        outputVal = filter.process(mixOscNoise,cutoff,resonance,filtertype,lfoFreq,lfoShape,noLFO);
                        left[i] +=  4 * outputVal * (lfoPan.process() * gain + gain);
                        right[i] += 4 * outputVal * ((-lfoPan.process() * gain
                                                      + gain));
                        // use a counter to read audio buffer continuously
                        if(counter > fileBuffer.getNumSamples())
                            counter = 0;
                        counter += 1;
                    break;
                    }
                }
            
                // if the envolope is off, clear notes and stop
                if (! envelope.isActive())
                {
                    //if envelope has finished, stop playing and free the voice
                    clearCurrentNote();
                    playing = false;
                    // put counter = 0 , other wise we won't get a new start of audio when press the key
                    counter = 0;
                    envelope.reset();
                }
                
            }
        }
    }
    /**
    @brief read in binary audio files difined
    @param a list of file buffers stored 16 audio files
    @note It is a little bit stupid to pass every single buffur in, which makes code so long. I have tried using raw and smart pointer, vectors to improve this, but I got memory leak problem
     each time. Optimizatioin will be done when I figrue it out.
    */
    void readInAudio(juce::AudioSampleBuffer _fileBuffer,juce::AudioSampleBuffer _fileBuffer1,juce::AudioSampleBuffer _fileBuffer2,juce::AudioSampleBuffer _fileBuffer3,juce::AudioSampleBuffer _fileBuffer4,juce::AudioSampleBuffer _fileBuffer5,juce::AudioSampleBuffer _fileBuffer6,juce::AudioSampleBuffer _fileBuffer7,juce::AudioSampleBuffer _fileBuffer8,juce::AudioSampleBuffer _fileBuffer9,juce::AudioSampleBuffer _fileBuffer10,juce::AudioSampleBuffer _fileBuffer11,juce::AudioSampleBuffer _fileBuffer12,juce::AudioSampleBuffer _fileBuffer13,juce::AudioSampleBuffer _fileBuffer14,juce::AudioSampleBuffer _fileBuffer15,juce::AudioSampleBuffer _fileBuffer16)
    {
        fileBuffer = _fileBuffer;
        fileBuffer1 = _fileBuffer1;
        fileBuffer2 = _fileBuffer2;
        fileBuffer3 = _fileBuffer3;
        fileBuffer4 = _fileBuffer4;
        fileBuffer5 = _fileBuffer5;
        fileBuffer6 = _fileBuffer6;
        fileBuffer7 = _fileBuffer7;
        fileBuffer8 = _fileBuffer8;
        fileBuffer9 = _fileBuffer9;
        fileBuffer10 = _fileBuffer10;
        fileBuffer11 = _fileBuffer11;
        fileBuffer12 = _fileBuffer12;
        fileBuffer13 = _fileBuffer13;
        fileBuffer14 = _fileBuffer14;
        fileBuffer15 = _fileBuffer15;
        fileBuffer16 = _fileBuffer16;
        
    }

    //--------------------------------------------------------------------------
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    /**
     Can this voice play a sound. I wouldn't worry about this for the time being

     @param sound a juce::SynthesiserSound* base class pointer
     @return sound cast as a pointer to an instance of YourSynthSound
     */
    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<MySynthSound*> (sound) != nullptr;
    }
    //--------------------------------------------------------------------------
private:
    //--------------------------------------------------------------------------
    // Set up any necessary variables here
    
    // the wave mode user choose to use
    float waveMode = 0.0f;
    //mix ratio between two oscillators
    float oscMix = 0.0f;
    //mix ratio between osciallator output and noise
    float noiseMix = 0.0f;
    //signal after noise mixing
    float mixOscNoise = 0.0f;
    //final output value
    float outputVal = 0.0f;
    
    //Should the voice be playing?
    bool playing = false;
    //set up an index to track the samples in the audio files, otherwis it will only read the first 512 samples
    int counter = 0;
    //midi frequency
    float freq = 0.0f;
    // overall gain control
    float gain = 0.0f;
    // gain for velosity
    float gainVol = 0.0f;
    
    /// envolope object and essential params ADSR
    juce::ADSR envelope;
    juce::ADSR::Parameters envParameters;
    //controllable parameters
    std::atomic<float>* attackParam;
    std::atomic<float>* decayParam;
    std::atomic<float>* sustainParam;
    std::atomic<float>* releaseParam;
    //current env value
    float envVal = 0.0f;
    
    ///fist osc set
    // shape of oscillators
    float oscChoice1 = 0.0f;
    // 4 types of osc
    SinOsc osc1_0;
    SquareOsc osc1_1;
    TriOsc osc1_2;
    SawOsc osc1_3;
    //osc final value after choose from one of oscillators
    float oscVal_1 = 0.0f;
    
    ///second osc set
    // shape of oscillators
    float oscChoice2 = 0.0f;
    // 4 types of osc
    SinOsc osc2_0;
    SquareOsc osc2_1;
    TriOsc osc2_2;
    SawOsc osc2_3;
    //osc final value after choose from one of oscillators
    float oscVal_2 = 0.0f;
    //store the mix oscllators
    float mixOsc;
    
    ///essential instance and params for phase modultaion
    PhaModulatedOsc phaseMod;
    float phaseModFre = 0.0f;
    //how many time does the seccessive modulation happen
    float phaseModDepth = 0.0f;
    //shape of modulator
    float phaseModType = 0.0f;
    //store the resault after modulation
    float modOutputVal = 0.0f;
    
    //random number
    juce::Random random;
    //noise
    float noise = 0.0f;
    //Noise noise=0.0f;
    ///essential instance and params for Filter
    
    Filters filter;
    //cutoff frequency for filter
    float cutoff = 0.0f;
    //resonance of filter
    float resonance = 0.0f;
    // type of filter
    float filtertype = 0.0f;
    // if lfo is trun on in filter
    float noLFO = 0.0f;
    //integer used to determine which lfo shape is currently selected
    int lfoShape = 1;
    // frequency of lfo
    float lfoFreq = 0.1f;
    // control the pan, moving between left to right and channel
    SinOsc lfoPan;
    float lfoFrePan;
    
    ///choose different sample type: drum, loop, fx, atoms
    float sampleType = 0.0f;
    //type of drum
    float drum = 0.0f;
    //type of loop
    float loop = 0.0f;
    float fx = 0.0f;
    float atmos = 0.0f;

    
    /// file manager, reader, buffers to read from audio files
    juce::AudioFormatManager formatManager;
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

    
    
};

