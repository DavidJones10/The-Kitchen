#include "flanger.h"
#include "dsp.h"

using namespace EZ_DSP;

void Flanger::Init(float sample_rate)
{
    sampleRate = sample_rate;
}

float Flanger::Process(float in)
{

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

}

void Flanger::setLfoDepth(float depth)
{
    depth = fclamp(depth, 0, .95f);
     
}

void Flanger::setLfoFrequency(float freq)
{

}

float Flanger::processLfoSin()
{

}

float Flanger::processLfoTri()
{

}
