#pragma once
#ifndef EZ_DSP_TREMOLO_H
#define EZ_DSP_TREMOLO_H
#include "dsp.h"
#include "oscillator.h"
namespace EZ_DSP
{

class Tremolo
{
public:
    Tremolo(){}
    ~Tremolo(){}
    /** initializes oscillator and internal values
     * @param sample_rate sample rate in processing context
     */
    void init(float sample_rate);
    /** processes input and returns tremolo-effected output
     * @param input float sample in sample by sample loop
     */
    float process(float input);
    /** Sets lfo rate for tremolo
     * @param rate expects value between 0 and 10Hz
     */
    void setRate(float rate);
    /** Sets lfo depth for tremolo
     * @param depth expects value between 0-1
     */
    void setDepth(float depth);
    /** Sets the waveform type for the lfo
     * @param waveform 0:Sin, 1:Tri, 2:Saw, 3:Square
     */
    void setWaveform(int waveform);


private:
    int waveType;
    float drywet;
    float dc_offset;
    Oscillator osc;

};
} // namespace EZ_DSP
#endif