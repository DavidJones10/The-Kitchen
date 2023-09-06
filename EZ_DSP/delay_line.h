#pragma once
#ifndef EZ_DSP_DELAY_LINE_H
#define EZ_DSP_DELAY_LINE_H
#include <stdlib.h>
#include <stdint.h>

namespace EZ_DSP
{
template <typename sampleType, size_t max_size>
class DelayLine 
{
public:
    DelayLine(){}
    ~DelayLine(){}
    void Init() {Reset();}
    void Reset() 
    {
        for (size_t i=0; i < max_size; i++)
        {
            delLine[i] = sampleType(0);
        }
        writePtr = 0;
        readPtr = 1;
    }
    inline void setDelay(size_t delay)
    {
        fracional = 0;
        readPtr = readPtr < max_size ? readPtr : max_size - 1;
    }
    inline void setDelay(float delay)
    {
        int32_t delayInt = static_cast<int32_t>(delay);
        fractional = delay - static_cast<float>(delayInt);
        readPtr = static_cast<size_t>(delayInt) < max_size ? delayInt : max_size - 1;
    }
    inline void write(const sampleType sample)
    {
        delLine[writePtr] = sample;
        writePtr = (writePtr - 1 + max_size) % max_size;
    }
    // Outputs interpolated sample from delay line.   
    //   Good if delay time is set in process block
    inline const sampleType read(float delSample)
    {
        int32_t delayInt = static_cast<int32_t>(delSample);
        float delFraction = delaySample - static_cast<float>(delayInt);
        const sampleType sample1 = delLine[(writePtr + delayInt) % max_size];
        const sampleType sample2 = delLine[(writePtr + delayInt + 1) % max_size];
        return lerp(sample1, sample2, delFraction);
    }
    // Outputs interpolated sample from delay line.
    //   Best for when delay time is set in different function
    inline const sampleType read()
    {
        const sampleType sample1 = delLine[(writePtr + delayInt) % max_size];
        const sampleType sample2 = delLine[(writePtr + delayInt + 1) % max_size];
        return lerp(sample1, sample2, fractional);
        
    }
    
private:
    float lerp(float sample1, float sample2, float inPhase)
        {return (1-inPhase) * sample1 + inPhase * sample2;}
    
    float fractional;
    size_t writePtr;
    size_t readPtr;
    sampleType delLine[max_size];
    
};
}
#endif
