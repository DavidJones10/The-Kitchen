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
        // Sets Delay from 0-1 scales to .1-7ms
        void setDelay(float delayInSamples);
        void setDelayMs(float delayMS);
        void setModMode(int mode);

    private:
        float feedback;
        float LfoDepth;
        float LfoFreq;
        float delay;
        float sampleRate;
        float LfoAmp;
        float LfoPhase;
        int8_t modType = 0;
        static constexpr int32_t maxDelaySize = 960; // .02 * 48000
        DelayLine<float,maxDelaySize> delLine;
        enum ModModes
        {
            SIN = 0,
            TRI = 1
        };

        float processLfoTri();
        float processLfoSin();
};
}
#endif

