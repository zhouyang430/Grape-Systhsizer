//
//  Oscillators.h
//  PointerTest
//
//  Created by tmudd on 06/02/2020.
//  Copyright © 2020 tmudd. All rights reserved.
//

#ifndef Oscillators_h
#define Oscillators_h

#include <cmath>

// PARENT phasor class
class Phasor
{
public:
    
    virtual ~Phasor() {}
    
    // Our parent oscillator class does the key things required for most oscillators:
    // -- handles phase
    // -- handles setters and getters for frequency and samplerate
    
    /// update the phase and output the next sample from the oscillator
    float process()
    {
        phase += phaseDelta;
        
        if (phase > 1.0f)
            phase -= 1.0f;
        
        return output(phase);
    }
    
    virtual float output(float p)
    {
        return p;
    }
    
    void setSampleRate(float SR)
    {
        sampleRate = SR;
    }
    void setFrequency(float freq)
    {
        frequency = freq;
        phaseDelta = frequency / sampleRate;
    }
    
private:
    float frequency;
    float sampleRate;
    float phase = 0.0f;
    float phaseDelta;
};
//==========================================


// CHILD Class
class TriOsc : public Phasor
{
    float output(float p) override
    {
        return fabsf(p - 0.5f) - 0.5f;
    }
};


// CHILD Class
class SinOsc : public Phasor
{
    float output(float p) override
    {
        return std::sin(p * 2.0 * 3.14159);
    }
};

// CHILD Class
class SquareOsc : public Phasor
{
public:
    float output(float p) override
    {
        float outVal = 0.5;
        if (p > pulseWidth)
            outVal = -0.5;
        return outVal;
    }
    void setPulseWidth(float pw)
    {
        pulseWidth = pw;
    }
private:
    float pulseWidth = 0.5f;
};
class SawOsc : public Phasor
{
    // redefine (override) the output function so that we can return a different function of the phase (p)
    float output(float p) override
    {
        return p + 1.0f;
    }
};

#endif /* Oscillators_h */
