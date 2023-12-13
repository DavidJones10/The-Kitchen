#include "phaser.h"
#include "dsp.h"

using namespace EZ_DSP;
// Phaser Engine
void PhaserEngine::init(float sample_rate)
{
    sample_rate = sampleRate;
    delLine.init();
    
    LfoAmp = 0.f;
    feedback = .3f;
    setFreq(200.f);
    delLine.setDelay(0.f);

    freqOffset = 30.f; // 30Hz
    delTime = 0.f;
    LfoPhase = 0.f;
    setLfoFrequency(.3f);
    setLfoDepth(.9f);
}

float PhaserEngine::process(float input)
{
    float lfoSig = processLfo();
    fonepole(delTime, sampleRate / (lfoSig + allpassFreq + freqOffset), .0001f);
    float out = delLine.allpass(input, delTime, .3f);
    return (1.f-dryWet) * input + dryWet * out;
}

void PhaserEngine::setFeedback(float fback)
{
    feedback = fclamp(fback, 0.f, .75f);
}

void PhaserEngine::setLfoFrequency(float frequency)
{
    LfoFreq = fclamp(0, 0.f, 10.f);
}

void PhaserEngine::setLfoDepth(float depth)
{
    LfoAmp = fclamp(depth,0.f, 1.f);
}

void PhaserEngine::setFreq(float apFreq)
{
    allpassFreq = fclamp(apFreq, 0.f, 20000.f);
}

void PhaserEngine::setDryWet(float wet)
{
    dryWet = fclamp(wet,0.f,1.f);
}

float PhaserEngine::processLfo()
{
    LfoPhase += LfoFreq;
    if (LfoPhase >= 1.0f) LfoPhase -= 2.0f; 
    return LfoAmp * sinf(TWOPI_F * LfoPhase) * allpassFreq;
}

// Actual Phaser effect

void Phaser::init(float sample_rate)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].init(sample_rate);
    }
    numPoles = 4;
}

float Phaser::process(float input)
{
    float out = 0.f;
    for (int i=0; i < numPoles; i++)
    {
        out += engines[i].process(input);
    } 
    return out;
}

void Phaser::setNumPoles(int poles)
{
    numPoles = clampInt(poles,1, 8);
}

void Phaser::setFeedback(float fback)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setFeedback(fback);
    }    
}

void Phaser::setLfoDepth(float depth)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setLfoDepth(depth);
    }
}

void Phaser::setLfoFrequency(float frequency)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setLfoFrequency(frequency);
    }
}

void Phaser::setDryWet(float wet)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setDryWet(wet);
    }
}


