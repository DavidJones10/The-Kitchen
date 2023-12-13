#include "oscillator.h"

using namespace EZ_DSP;

void Oscillator::init(float sample_rate)
{
    sampleRate =  sample_rate;
    frequency      = 100.0f;
    amplitude       = 0.5f;
    pulseWidth        = 0.5f;
    phase             = 0.0f;
    phaseInc =          getPhaseInc(frequency);
    waveType  = SIN;
    endOfCycle_             = true;
    endOfRise_             = true;
}

float Oscillator::process()
{
    float output, t;
    switch (waveType)
    {
    case SIN:
        output = sinf(phase * TWOPI_F);
        break;
    case TRI:
        t = -1.0f + (2.0f * phase);
        output = 2.0f * (fabsf(t) - 0.5f);
        break;
    case SAW:
        output = -1.0f * ((phase * 2.0f) - 1.0f);
        break;
    case SQUARE:
        output = phase < pulseWidth ? (1.0f) : -1.0f;
        break;
    default:
        output = 0.f;
        break;
    }

    phase += phaseInc;

    if (phase > 1.0f)
    {
        phase -= 1.f;
        endOfCycle_ = true;
    }
    else endOfCycle_ = false;

    endOfRise_ = (phase - phaseInc < .5f && phase >= .5f);
    
    return output * amplitude;
}

