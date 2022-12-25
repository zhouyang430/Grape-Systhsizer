/**
@file Filters.h
@brief Countains Filters class, support all filter, low filter, high filter, band filter, notch filter
@author B200097
@version 1.0
@date 13/12/2022
*/
#include "Oscillators.h"
#include <math.h>
/**
@class Filters
@brief support all filter, low filter, high filter, band filter, notch filter
*/
class Filters
{
public:
    
    /**
     @brief set samplerate in Hz
     @param _sampleRate the sample rate in Hz
     */
    void setSampleRate(float _sampleRate)
    {
        sampleRate = _sampleRate;
        
        filterLFO1.setSampleRate(sampleRate);
        filterLFO2.setSampleRate(sampleRate);
        filterLFO3.setSampleRate(sampleRate);
        filterLFO4.setSampleRate(sampleRate);
        // replace with setRate function
        
    }
    /**
     @brief reset filter
     */
    void resetInnerFilter(){
        filter.reset();
    }
    /**
    @brief Mix the oscs together and put them through the modulating filter,
    @param input the signal to filter
    @param cutoff the cutoff frequency
    @param resonance the resonance
    @param filterType the filterType
    @param freqLFO the freq of LFO
    @param lfoType shape if lfo
    @return noLFO use lfo for cutoff changing or not
     */
    float process(float input,float cutoff, float resonance,float filterType,float freqLFO, float lfoType,float noLFO)
    {
        // set up frequency fo
        filterLFO1.setFrequency(freqLFO);
        filterLFO2.setFrequency(freqLFO);
        filterLFO3.setFrequency(freqLFO);
        filterLFO4.setFrequency(freqLFO);
        
        // using lfo to change cufoff frequency
        if(int(noLFO)==1)
        {
            switch(int(lfoType))
            {
                case 0:
                    cutoffFreq = filterLFO1.process() * 1000 + 1200;
                    break;
                case 1:
                    cutoffFreq = filterLFO2.process() * 1000 + 1200;
                case 2:
                    cutoffFreq = filterLFO3.process() * 1000 + 1200;
                    break;
                case 3:
                    cutoffFreq = filterLFO4.process() * 1000 + 1200;
                    break;

            }
        }
        // linearly change the cutoff frequency
        if(int(noLFO)==0)
        {
            cutoffFreq = cutoff;
        }
    
        switch(int(filterType))
        {
            case 0:
                filter.setCoefficients(juce::IIRCoefficients::makeAllPass(sampleRate, cutoffFreq,resonance));
                break;
            case 1:
                filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, cutoffFreq,resonance));
                break;
            case 2:
                filter.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, cutoffFreq,resonance));
                break;
            case 3:
                filter.setCoefficients(juce::IIRCoefficients::makeNotchFilter(sampleRate, cutoffFreq,resonance));
                break;
            case 4:
                filter.setCoefficients(juce::IIRCoefficients::makeBandPass(sampleRate, cutoffFreq,resonance));
                break;
        }
        float filterOut = filter.processSingleSampleRaw(input);
        return filterOut;
        
    }
private:
    // sample rate
    float sampleRate = 0.0f;
    // filter
    juce::IIRFilter filter;
    //LFO Shape 1 - Sinusoidal
    //cutoff frequency
    float cutoffFreq = 0.0f;
    SinOsc filterLFO1;
    //LFO Shape 2 - Triangle
    TriOsc filterLFO2;
    //LFO Shape 3 - Saw
    SawOsc filterLFO3;
    //LFO Shape 4 - Square
    SquareOsc filterLFO4;

};

