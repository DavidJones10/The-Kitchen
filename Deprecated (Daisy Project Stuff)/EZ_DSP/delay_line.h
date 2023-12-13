#pragma once
#ifndef EZ_DSP_DELAY_LINE_H
#define EZ_DSP_DELAY_LINE_H
#include <stdlib.h>
#include <stdint.h>

namespace EZ_DSP
{
template <typename sampleType, size_t maxSize>
class DelayLine 
{
public:
    DelayLine(){}
    ~DelayLine(){}
    
    // Initializes delay line
    void init() {reset();}
    
    // Resets buffer, sets all values to 0
    void reset() 
    {
        for (size_t i=0; i < maxSize; i++)
        {
            delLine[i] = sampleType(0);
        }
        writePtr = 0;
        readPtr = 1;
    }
    
    // sets delay for size_t input, assumes unchanging value
    inline void setDelay(size_t delay)
    {
        fractional = 0;
        readPtr = readPtr < maxSize ? readPtr : maxSize - 1;
    }

    /** sets delay for float input, creates fractional delay
        for changing value
        @param delay delay in samples
    */
    inline void setDelay(float delay)
    {
        int32_t delayInt = static_cast<int32_t>(delay);
        fractional = delay - static_cast<float>(delayInt);
        readPtr = static_cast<size_t>(delayInt) < maxSize ? delayInt : maxSize - 1;
    }
    // Writes sample to delay line
    inline void write(const sampleType sample)
    {
        delLine[writePtr] = sample;
        writePtr = (writePtr - 1 + maxSize) % maxSize;
    }

    // Outputs interpolated sample from delay line.   
    //   Good if delay time is set in process block
    inline const sampleType read(float delSample)
    {
        int32_t delayInt = static_cast<int32_t>(delSample);
        float delFraction = delSample - static_cast<float>(delayInt);
        const sampleType sample1 = delLine[(writePtr + delayInt) % maxSize];
        const sampleType sample2 = delLine[(writePtr + delayInt + 1) % maxSize];
        return lerp(sample1, sample2, delFraction);
    }

    // Outputs interpolated sample from delay line.
    //   Best for when delay time is set in different function
    inline const sampleType read()
    {
        int32_t delayInt = static_cast<int32_t>(readPtr);
        const sampleType sample1 = delLine[(writePtr + delayInt) % maxSize];
        const sampleType sample2 = delLine[(writePtr + delayInt + 1) % maxSize];
        return lerp(sample1, sample2, fractional);
    }

    /** Processes input and returns allpass filtered output
    --coefficient value meanings/usage:
    --- 0-1: phase shift without much effect on amplitude
    --- 1: simple delay line with 'delay' number of samples
    --- negative: introduces feedback, useful for reverb or comb filters
    */
    inline const sampleType allpass(const sampleType sample, size_t delay, const sampleType coefficient)
    {
        sampleType read = delLine[(writePtr+delay) % maxSize];
        sampleType writeSample = sample + coefficient * read;
        write(writeSample);
        return -writeSample * coefficient + read;
    }
    
private:
    float lerp(float sample1, float sample2, float inPhase)
        {return (1-inPhase) * sample1 + inPhase * sample2;}
    
    float fractional;
    size_t writePtr;
    size_t readPtr;
    sampleType delLine[maxSize];
    
};
}
#endif
