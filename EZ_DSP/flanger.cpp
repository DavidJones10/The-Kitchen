#include "flanger.h"
#include "dsp.h"

using namespace EZ_DSP;

void Flanger::Init(float sample_rate)
{
    sampleRate = sample_rate;
    LfoPhase = 0.f;
}

float Flanger::Process(float in)
{
    float lfoSignal;
    if (modType == 0) lfoSignal = processLfoSin();
    else lfoSignal = processLfoTri();
    delLine.setDelay(1.f + lfoSignal + delay);
    float out = delLine.read();
    delLine.write(in + out * feedback);
    return in *(1-dryWet) + out*dryWet; 
}

void Flanger::setFeedback(float fback)
{
    feedback = (fback, 0, 1);
    feedback *= .97f; // keeps feedback from being too high
}

void Flanger::setDelay(float del)
{
    del = (.1 + del * 6.9);
    setDelayMs(del);
}

void Flanger::setDelayMs(float delayMs)
{
    delayMs = fmax(.1, delayMs);
    delay = delayMs * .001f * sampleRate;
    LfoAmp = fmin(LfoAmp, delay);
}

void Flanger::setLfoDepth(float depth)
{
    depth = fclamp(depth, 0, .95f);
    LfoAmp = depth * delay;
}

void Flanger::setLfoFrequency(float freq)
{
    if (modType==0) LfoFreq = freq/sampleRate;
    else
    {
        freq = freq * 4.f / sampleRate;
        freq *= LfoFreq < 0.f ? -1.f : 1.f; 
        LfoFreq = fclamp(freq,-.25f, .25f);
    }
}

void Flanger::setDryWet(float wet)
{
    dryWet = wet;
}

float Flanger::processLfoSin()
{
    LfoPhase += LfoFreq;
    if (LfoPhase >= 1.f) LfoPhase -= 1;
    return LfoAmp * sin(TWOPI_F * LfoPhase);
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
