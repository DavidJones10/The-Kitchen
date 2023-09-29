#include "oscillator.h"

using namespace EZ_DSP;

void Oscillator::init(float sample_rate)
{
    sampleRate =  sample_rate;
    frequency      = 100.0f;
    amplitude       = 0.5f;
    pulsWidth        = 0.5f;
    phase             = 0.0f;
    phaseInc =          getPhaseInc(frequency);
    waveType  = SIN;
    endOfCycle             = true;
    endOfRise             = true;

}

