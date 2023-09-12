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
         * @param sample_rate sample rate for internal calculations
         */
        void init(float sample_rate);
        /** Processes chorus engine
            \param input float input from audio engine
        */
        float process(float input);
        
        /** Sets feedback for internal delay 
            \param fback expects 0-1 
        */
        void setFeedback(float fback);

        /** Sets amplitude of modulator
            \param depth expects 0-1 
        */
        void setLfoDepth(float depth);

        /** Sets modulation rate
            \param frequency expects 0-10 Hz
        */
        void setLfoFrequency(float frequency);

        /** Sets internal delay using float range
            \param delayScaled expects 0-1 Scales to 10-35ms
        */
        void setDelay(float delayScaled);

        /** Sets internal delay in ms
            \param delayMS expects 10-35ms
        */
        void setDelayMs(float delayMS);

        /** Sets dry wet for chorus engine
            \param wet expects 0-1
        */
        void setDryWet(float wet);
        
        /** Sets offset for chorus, only useful if you have 2 chorusEngines
            \param offset offset for chorus modulator, expects 0-1
         */
        void addPhaseOffset(float offset);
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
        float processLfo();
};

class Chorus
{
    public:
        Chorus(){}
        ~Chorus(){}
        /** Initializes chorus(es)
            \param sample_rate sample rate for internal calculations
        */
        void init(float sample_rate);

        /** Processes input and returns effected output 
         *  Returns left channel only 
            \param input sample to be processed and effected
        */
        float process(float input);

        // Returns value of left channel of effected signal
        // Must be called after process()
        float getLeft();

        // Returns value of right channel of effected signal
        // Must be called after process()
        float getRight();

        /** Sets pan for each channel individually
            \param panLeft expects 0-1, 0 is left, 1 is right
            \param panRight expects 0-1, 0 is left, 1 is right
        */
        void setPan(float panLeft, float panRight);

        /** Sets pan for both channels 
            \param pan expects 0-1, 0 is left, 1 is right
        */
        void setPan(float pan);

        /** Sets lfo depth for each channel independently
            \param depthLeft expects 0-1
            \param depthRight expects 0-1
        */
        void setLfoDepth(float depthLeft, float depthRight);

        /** Sets lfo depth for both channels
            \param depth  expects 0-1
        */
        void setLfoDepth(float depth);

        /** Sets lfo frequency for each channel independently
            \param freqLeft expects 0-10Hz
            \param freqRight expects 0-10Hz
        */
        void setLfoFreq(float freqLeft, float freqRight);

        /** Sets lfo frequency for both channels
            \param freq expdects 0-10Hz
        */
        void setLfoFreq(float freq);

        /** Sets internal delay for each channel individually 
         *    using float range
            \param delayLeft 
            \param delayRight
        */
        void setDelay(float delayLeft, float delayRight);

        /** Sets internal delay for both channels using float range
            \param delay expects 0-1
        */
        void setDelay(float delay);

        /** Sets internal delay in ms for both channels
            \param msLeft expects 10-35ms
            \param msRight expects 10-35ms
        */
        void setDelayMs(float msLeft, float msRight);

        /** Sets internal delay in ms for both channels 
            \param ms expects 10-35ms
        */
        void setDelayMs(float ms);

        /** Sets feedback for each channelsindependently
            \param fbackLeft expects 0-1
            \param fbackRight expects 0-1
        */
        void setFeedback(float fbackLeft, float fbackRight);

        /** Sets feedback for both channels 
            \param fback  expects 0-1
        */
        void setFeedback(float fback);

        /** Sets offset between Left and Right channel for stereo chorus
            \param offset expects 0-1
         */
        void setPhaseOffset(float offset);
        
        /** Sets dry/wet for both channels
            \param wet expects 0-1
         */
        void setDryWet(float wet);

    private:
        ChorusEngine engines[2];
        float gainFraction;
        float pans[2];
        float signalL, signalR;
        float phaseOffset;
};

}
#endif