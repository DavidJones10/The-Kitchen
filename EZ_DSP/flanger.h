#pragma once
#ifndef EZ_DSP_FLANGER_H
#define EZ_DSP_FLANGER_H

#include "delay_line.h"

namespace EZ_DSP
{
class Flanger 
{
    public:
        void Init(float sample_rate);
        float Process(float input);
        void setFeedback(float feedback);
        void setLfoDepth(float depth);
        void setLfoFrequency(float frequency);
        void setDelay(float delayInSamples);
        void setDelayMs(float delayMS);
    private:
        float feedback;
        float LfoDepth;
        float LfoFreq;
        float delay;
        float sampleRate;
        float processLfo();
        static constexpr int32_t maxDelaySize = 960;
        DelayLine<float,maxDelaySize> delLine;
};
#endif













}

