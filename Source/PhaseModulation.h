/**
@file PhaseModulation.h
@brief perform phase modulation, customize the params for modulator
@author B200097
@version 1.0
@date 13/12/2022
*/

#include "Oscillators.h"
#include <math.h>
/**
@class PhaModulatedOsc
@brief perform phase modulation, customize the params for modulator
*/
class PhaModulatedOsc
{
public:
    
    /**
     @brief set samplerate in Hz
     @param _sampleRate the sample rate in Hz
     */
    void setSampleRate(float _sampleRate)
    {
        sampleRate = _sampleRate;
        carrier.setSampleRate(sampleRate);
        osc1_0.setSampleRate(sampleRate);
        osc1_1.setSampleRate(sampleRate);
        osc1_2.setSampleRate(sampleRate);
        osc1_3.setSampleRate(sampleRate);
        
    }
    
    /**
     @brief set samplerate in Hz. Best to have called setSampleRate() first
     @param freq the frequency of the carrier
    */
    void setFrequencyC(float _freq)
    {
        freqC = _freq;
        carrier.setFrequency(_freq);
    }
    /**
     @brief set samplerate in Hz. Best to have called setSampleRate() first
     @param freq the frequency of the modulator
    */
    void setFrequencyM(float freq)
    {
        freqM = freq;
        osc1_0.setFrequency(freq);
        osc1_1.setFrequency(freq);
        osc1_2.setFrequency(freq);
        osc1_3.setFrequency(freq);

    }

    /**
    @brief Implement modulation to a sinewave which frequency is the Midi note, modulator can be customized
    @param modIndex the modulation index
    @param modDepth the self-modulation times , to control create richer sound
    @param modType shape of modulator
    @return the modulated oscillator
     */
    float process(float modIndex,int modDepth,int modType)
    {   modIndex = modIndex;
        int count = modDepth - 1;
        
        // get the pahase of carrier by arc sin function
        float ocsCarry = asin(carrier.process()) * 2 * 3.1415926;
        // get osc values and sum them together
        switch(int(modType))
        {
            case 0:
                oscVal_1= sin(ocsCarry + osc1_0.process() * modIndex);
                break;
            case 1:
                oscVal_1= sin(ocsCarry + osc1_1.process() * modIndex);
                break;
            case 2:
                oscVal_1= sin(ocsCarry + osc1_2.process() * modIndex);
                break;
            case 3:
                oscVal_1= sin(ocsCarry + osc1_3.process() * modIndex);
                break;
        }
        float oscsOut = sin(ocsCarry + oscVal_1 * modIndex);
        // set filter lfo
        // DX7 effect, modulation agian and again
        while(count > 0){
            oscsOut = sin( ocsCarry + oscsOut * modIndex);
            count -= 1;
        }
        
        return oscsOut;
        
    }
private:
    // frequency of carrier
    float freqC = 0.0f;
    //frequency of modulator
    float freqM = 0.0f;
    // sample rate
    float sampleRate = 0.0f;
    //store the resul for modultaion
    float oscVal_1 = 0.0f;
    // modIndex
    float modIndex = 0.0f;
    // carrier, square osc from Oscillators.h header file
    SinOsc carrier;
    // modulator shape
    SinOsc osc1_0;
    SquareOsc osc1_1;
    TriOsc osc1_2;
    SawOsc osc1_3;
    
   
    
    
    
};
