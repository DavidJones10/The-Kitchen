#pragma once
#ifndef EZ_DSP_FLANGER_H
#define EZ_DSP_FLANGER_H

#include "delay_line.h"

namespace EZ_DSP
{
class Flanger 
{
    public:
        /** Initializes variables for Flanger effect
            \param sample_rate sample rate for audio engine
        */
        void init(float sample_rate);

        /** Processes input and returns flanger-effected output
         * \param input input sample 
        */
        float process(float input);

        /** Sets feedback for internal delay 
            \param fback expects 0-1 float value
        */
        void setFeedback(float fback);

        /** Sets amplitude of modulator
            \param depth expects 0-1 float value
        */
        void setLfoDepth(float depth);

        /** Sets modulation rate
            \param frequency Hz value between 0 and 5Hz
        */
        void setLfoFrequency(float frequency);

        /** Sets internal delay as a float range
            \param delayInSamples expects value from 0-1 Scales to .1-7ms
        */
        void setDelay(float delayInSamples);

        /** Sets internal delay in ms
            \param delayMS expects value in ms between .1 and 10ms
        */
        void setDelayMs(float delayMS);

        /** Modulator wave type mode selector. 0: SIN; 1: TRI
            \param mode Defaults to 0, outOfRange = 0
        */
        void setModMode(int mode);

        /** Sets dry wet for flanger effect
            \param wet expects value from 0-1
        */
        void setDryWet(float wet);

    private:
        float feedback;
        float LfoDepth;
        float LfoFreq;
        float delay;
        float sampleRate;
        float LfoAmp;
        float LfoPhase;
        int8_t modType = 0;
        float dryWet;
        static constexpr int32_t maxDelaySize = 960; // .02 * 48000 max size of 20ms
        DelayLine<float,maxDelaySize> delLine;
        enum ModModes
        {
            SIN = 0,
            TRI = 1
        };
        // processes LFO as triangle waveform; used in Process() function
        float processLfoTri();

        // processes LFO as sine waveform; used in Process() function
        float processLfoSin();
};
}
#endif

