#ifndef EZ_DSP_OSCILLATOR_H
#define EZ_DSP_OSCILLATOR_H
#include "dsp.h"

namespace EZ_DSP
{
class Oscillator
{
public:
    Oscillator(){}
    ~Oscillator(){}
    enum WaveType
    {
        SIN, TRI, SAW, SQUARE, LAST
    };
   
    void init(float sample_rate);

    float process();

    void setAmplitude(float amp) {amplitude = amp;}

    void setFreq(float freq) {frequency = freq;}

    void setWaveType(u_int8_t type) {waveType = type < LAST ? type : SIN;}
    
private:
    float amplitude, frequency, pulsWidth, sampleRate, phase, phaseInc;
    bool endOfRise, endOfCycle;
    u_int8_t waveType;

    float getPhaseInc(float freq) {return freq/sampleRate;};



};

}// namespace EZ_DSP
#endif