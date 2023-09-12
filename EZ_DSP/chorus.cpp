#include "chorus.h"
#include "dsp.h"
#include "math.h"

using namespace EZ_DSP;
// Chorus Engine: use 2 for normal stereo effect in chorus
void ChorusEngine::init(float sample_rate)
{
    sampleRate = sample_rate;
    delLine.Init();
    LfoAmp  = 0.f;
    feedback = .2f;
    setDelay(.75);

    LfoPhase = 0.f;
    setLfoFrequency(.3f);
    setLfoDepth(.9f);
}

float ChorusEngine::process(float input)
{
    float lfoSig = processLfo();
    delLine.setDelay(lfoSig*delay);
    float out = delLine.read();
    delLine.write(input + out * feedback);
    
    return input *(1.f-dryWet) + out*dryWet; 
}

void ChorusEngine::setLfoDepth(float depth)
{
    depth = fclamp(depth, 0.f, .98f);
    LfoAmp = depth * delay;
}

void ChorusEngine::setFeedback(float fback)
{
    feedback = (fback, 0.f, .99f);
    feedback *= .97f; // keeps feedback from being too high
}

void ChorusEngine::setLfoFrequency(float freq)
{
    LfoFreq = fclamp(LfoFreq, 0.f, 10.f);
    LfoFreq = freq/sampleRate;
}

void ChorusEngine::addPhaseOffset(float offset)
{
    offset = fclamp(offset, 0.f, 1.f);
    LfoPhase += offset;
}

void ChorusEngine::setDelay(float delayScaled)
{
    delayScaled = fclamp(delayScaled, 0.f, 1.f);
    delayScaled = (10.1f + delayScaled * 34.9f); //scaled between 10 and 35ms
    setDelayMs(delayScaled);
}

void ChorusEngine::setDelayMs(float ms)
{
    ms = fclamp(ms, 10.f, 35.f);
    delay = ms * .001f * sampleRate;
    LfoAmp = fmin(LfoAmp, delay);
}

void ChorusEngine::setDryWet(float wet)
{
    dryWet = fclamp(wet,0,.99f);
}

float ChorusEngine::processLfo()
{
    LfoPhase += LfoFreq;
    if (LfoPhase >= 1.f) LfoPhase -= 1.f;
    return LfoAmp * std::sin(TWOPI_F * LfoPhase);
}


//=================================================================
// Actual stereo chorus
void Chorus::init(float sample_rate)
{
    sampleRate = sample_rate;
    engines[0].init(sample_rate);
    engines[1].init(sample_rate);
    setPan(.25f, .75f);
    signalL = signalR = 0.f;
    gainFraction = .5f;
}

float Chorus::process(float input)
{
    signalL = 0.f;
    signalR = 0.f;
    for (int i=0; i < 2; i++)
    {
        float signal = engines[i].process(input);
        signalL += (1.f-pans[i]) * signal;
        signalR += pans[i] * signal;
    }
    signalL *= gainFraction;
    signalR *= gainFraction;
    return signalL;
}

float Chorus::getLeft()
{
    return signalL;
}

float Chorus::getRight()
{
    return signalR;
}

void Chorus::setPan(float panLeft, float panRight)
{
    pans[0] = fclamp(panLeft, 0.f, 1.f);
    pans[1] = fclamp(panRight, 0.f, 1.f);
}

void Chorus::setPan(float pan)
{
    setPan(pan, pan);
}

void Chorus::setLfoDepth(float depthLeft, float depthRight)
{
    engines[0].setLfoDepth(depthLeft);
    engines[1].setLfoDepth(depthRight);
}

void Chorus::setLfoDepth(float depth)
{
    setLfoDepth(depth, depth);
}

void Chorus::setLfoFreq(float freqLeft, float freqRight)
{
    engines[0].setLfoFrequency(freqLeft);
    engines[1].setLfoFrequency(freqRight);
}

void Chorus::setLfoFreq(float freq)
{
    setLfoFreq(freq, freq);
}

void Chorus::setDelay(float delayLeft, float delayRight)
{
    engines[0].setDelay(delayLeft);
    engines[1].setDelay(delayRight);
}

void Chorus::setDelay(float delay)
{
    setDelay(delay, delay);
}

void Chorus::setDelayMs(float msLeft, float msRight)
{
    engines[0].setDelayMs(msLeft);
    engines[1].setDelayMs(msRight);
}

void Chorus::setDelayMs(float ms)
{
    setDelayMs(ms, ms);
}

void Chorus::setFeedback(float fbackLeft, float fbackRight)
{
    engines[0].setFeedback(fbackLeft);
    engines[1].setFeedback(fbackRight);

}

void Chorus::setFeedback(float fback)
{
    setFeedback(fback, fback);
}

void Chorus::setPhaseOffset(float offset)
{
    engines[1].addPhaseOffset(offset);
}

void Chorus::setDryWet(float wet)
{
    engines[0].setDryWet(wet);
    engines[1].setDryWet(wet);
}