#pragma once
#ifndef EZ_DSP_CHORUS_H
#define EZ_DSP_CHORUS_H

#include "delay_line.h"

namespace EZ_DSP
{
class ChorusEngine
{
    public:
        ChorusEngine(){}
        ~ChorusEngine(){}
        /** Initializes the chorus engine
         * @param sample_rate 
         */
        void init(float sample_rate);
        /** Processes chorus engine
            \param input float input from audio engine
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
            \param frequency Hz value between -20 and 20 Hz
        */
        void setLfoFrequency(float frequency);

        /** Sets internal delay as a float range
            \param delayScaled expects value from 0-1 Scales to .1-7ms
        */
        void setDelay(float delayScaled);

        /** Sets internal delay in ms
            \param delayMS expects value in ms between .1 and 7ms
        */
        void setDelayMs(float delayMS);

        /** Modulator wave type mode selector. 0: SIN; 1: TRI
            \param mode Defaults to 0, outOfRange = 0
        */
        void setModMode(int mode);

        /** Sets dry wet for chorus engine
            \param wet expects value from 0-1
        */
        void setDryWet(float wet);
    private:
        float feedback;
        float LfoFreq;
        float delay;
        float sampleRate;
        float LfoAmp;
        float LfoPhase;
        float dryWet;
        int8_t motType = 0;
        static constexpr int32_t maxDelaySize = 2400; // 50ms max
        DelayLine<float, maxDelaySize> delLine;
        enum ModModes
        {
            SIN = 0,
            TRI = 1
        };
        
        float processLfoSin();
        float processLfoTri();
};

class Chorus
{
    public:
        Chorus(){}
        ~Chorus(){}
        /** 
            \param sample_rate 
        */
        void init(float sample_rate);

        /** 
            \param input
        */
        float process(float input);

        /**  */
        float getLeft();

        /**  */
        float getRight();

        /** 
            \param panLeft
            \param panRight
        */
        void setPan(float panLeft, float panRight);

        /** 
            \param pan 
        */
        void setPan(float pan);

        /** 
            \param depthLeft 
            \param depthRight
        */
        void setLfoDepth(float depthLeft, float depthRight);

        /** 
            \param depth 
        */
        void setLfoDepth(float depth);

        /** 
            \param freqLeft 
            \param freqRight 
        */
        void setLfoFreq(float freqLeft, float freqRight);

        /** 
            \param freq 
        */
        void setLfoFreq(float freq);

        /** 
            \param delayLeft 
            \param delayRight
        */
        void setDelay(float delayLeft, float delayRight);

        /** 
            \param delay 
        */
        void setDelay(float delay);

        /** 
            \param msLeft 
            \param msRight 
        */
        void setDelayMs(float msLeft, float msRight);

        /** 
            \param ms 
        */
        void setDelayMs(float ms);

        /** 
            \param fbackLeft
            \param fbackRight
        */
        void setFeedback(float fbackLeft, float fbackRight);

        /** 
            \param fback 
        */
        void setFeedback(float fback);

    private:
        ChorusEngine engines[2];
        float gainFraction;
        float pans[2];
        float signalL, signalR;
};

}
#endif