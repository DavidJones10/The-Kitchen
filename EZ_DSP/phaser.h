#pragma once
#ifndef EZ_DSP_PHASER_H
#define EZ_DSP_PHASER_H

#include "delay_line.h"

namespace EZ_DSP
{
class PhaserEngine
{
    public:
        PhaserEngine(){}
        ~PhaserEngine(){}
        /** Initializes the phaser engine
         * @param sample_rate sample rate for internal calculations
         */
        void init(float sample_rate);
        /** Processes phaser engine
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
        
        /** Set the allpass frequency
        \param ap_freq Frequency in Hz.
        */
        void setFreq(float allpassFreq);
  
        /** Sets dry wet for chorus engine
            \param wet expects 0-1
        */
        void setDryWet(float wet);
    private:
        float feedback; 
        float LfoFreq; // LFO Rate
        float LfoAmp; // LFO Depth
        float LfoPhase; // LFO Phase
        float sampleRate;
        float dryWet;
        float freqOffset; // 
        float allpassFreq; 
        static constexpr int32_t maxDelaySize = 2400; // 50ms max
        DelayLine<float, maxDelaySize> delLine;
        float processLfo();

};

class Phaser
{
    public:
        /** Initializes Phaser effect
          @param sample_rate sample rate for audio playback
         */
        void init(float sample_rate);

        /** Processes input sample, returns phaser effected audio
         * @param input sample to be processed
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
        
        /** Set the allpass frequency
        \param ap_freq Frequency in Hz.
        */
        void setFreq(float allpassFreq);
  
        /** Sets dry wet for chorus engine
            \param wet expects 0-1
        */
        void setDryWet(float wet);

    private:
        PhaserEngine engines[8];


};
}
#endif