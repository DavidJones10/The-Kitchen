#include "flanger.h"
#include "dsp.h"

using namespace EZ_DSP;

void Flanger::init(float sample_rate)
{
    sampleRate = sample_rate;
    modType = 0;
    setModMode(0);
    setFeedback(.2f);
    delLine.Init();
    LfoAmp = 0.f;
    setDelay(.75f);
    LfoPhase = 0.f;
    setLfoFrequency(.3f);
    setLfoDepth(.9f);
}

float Flanger::process(float in)
{
    float lfoSignal;
    if (modType == 0) lfoSignal = processLfoSin();
    else lfoSignal = processLfoTri();

    delLine.setDelay(1.f + lfoSignal + delay);
    float out = delLine.read();
    delLine.write(in + out * feedback);
    
    return in *(1.f-dryWet) + out*dryWet; 
}

void Flanger::setFeedback(float fback)
{
    feedback = (fback, 0.f, 1.f);
    feedback *= .97f; // keeps feedback from being too high
}

void Flanger::setDelay(float del)
{
    del = fclamp(del, 0.f, 1.f);
    del = (.1f + del * 6.9f);
    setDelayMs(del);
}

void Flanger::setDelayMs(float delayMs)
{
    delayMs = fclamp(delayMs, .1, 10.f);
    delay = delayMs * .001f * sampleRate;
    LfoAmp = fmin(LfoAmp, delay);
}

void Flanger::setLfoDepth(float depth)
{
    depth = fclamp(depth, 0.f, .95f);
    LfoAmp = depth * delay;
}

void Flanger::setLfoFrequency(float freq)
{
    freq = fclamp(freq, .1f, 5.f);
    if (modType==0) LfoFreq = freq/sampleRate; // if SIN modulator
    else                                       // if TRI modulator
    {
        freq = freq * 4.f / sampleRate;
        freq *= LfoFreq < 0.f ? -1.f : 1.f; 
        LfoFreq = fclamp(freq,-.25f, .25f);
    }
}

void Flanger::setDryWet(float wet)
{
    dryWet = fclamp(wet,0,.99f);
}

void Flanger::setModMode(int mode)
{
    if (mode != 0 && mode != 1) mode = 0;
    else mode = mode;
    modType = mode;
}

float Flanger::processLfoSin()
{
    LfoPhase += LfoFreq;
    if (LfoPhase >= 1.f) LfoPhase -= 1.f;
    return LfoAmp * sinf(TWOPI_F * LfoPhase);
}

float Flanger::processLfoTri()
{
    LfoPhase += LfoFreq;
    if (LfoPhase > 1.f)
    {
        LfoPhase = 1.f -(LfoPhase -1.f);
        LfoPhase *= -1.f;
    }
    else if (LfoPhase < -1.f)
    {
        LfoPhase = -1.f - (LfoPhase + 1.f); 
        LfoPhase *= -1.f;
    }
    return LfoPhase * LfoAmp;
}
