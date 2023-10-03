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
    /** Initializes the oscillator
     * @param sample_rate sample rate for audio playback
     */
    void init(float sample_rate);
    // Processes the oscillator; should be called on a sample by sample basis
    float process();
    /** Sets amplitude for the oscillator
     * @param amp amplitude for the oscillator
     */
    inline void setAmplitude(float amp) {amplitude = amp;}
    /** Sets frequency of the oscillator
     * @param freq frequency in Hz
     */
    inline void setFreq(float freq) {frequency = freq;}
    /** Sets the wave type for the oscillator
     * @param type wave type- 1:SIN, 2:TRI, 3:SAW, 4:SQUARE
     */
    inline void setWaveType(u_int8_t type) {waveType = type < LAST ? type : SIN;}
    /** Sets pulse width for square waveform 
     * @param pw expects 0-1
     */
    inline void setPulseWidth(float pw) {pulseWidth = fclamp(pw, 0.f, 1.f);}
    // bool that returns true if the oscillator phase is rising
    inline bool isRising() {return phase < .5f;}
    // bool that returns true if the oscillator phase is falling
    inline bool isFalling() {return phase >= .5f;}
    /** Adds given phase to current internal phase
     * @param phase_ phase to add to current value
     */
    inline void addPhase(float phase_) {phase += phase_;}
    /** Resets phase to given value
     * @param phase_ phase to reset to, defaults to 0
     */
    inline void resetPhase(float phase_ = 0) {phase = phase_;}
    // returns true when the phase stops rising
    inline bool endOfRise() {return endOfRise_;}
    // returns true when a phase cycle is completed
    inline bool endOfCycle() {return endOfCycle_;}
    
private:
    float amplitude, frequency, pulseWidth, sampleRate, phase, phaseInc;
    bool endOfRise_, endOfCycle_;
    u_int8_t waveType;

    float getPhaseInc(float freq) {return freq/sampleRate;};

};

}// namespace EZ_DSP
#endif