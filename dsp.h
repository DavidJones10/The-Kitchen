#pragma once
#ifndef EZ_DSP_UTIL
#define EZ_DSP_UTIL
#include <cassert>
#include <cstdint>
#include <random>
#include <cmath>

#define PI_F 3.1415927410125732421875f
#define TWOPI_F (2.0f * PI_F)
#define HALFPI_F (PI_F * 0.5f)
namespace EZ_DSP 
{
    // fast maximum and minimum for floats, uses assembly if arm architecture
    inline float fmax(float a, float b)
    {
        float max;
        #ifdef __arm__
            asm("vmaxnm.f32 %[d], %[n], %[m]" : [d] "=t"(max) : [n] "t"(a), [m] "t"(b) :);
        #else
            max = (a > b) ? a : b;
        #endif // __arm__
            return max;
    }

    inline float fmin(float a, float b)
    {
        float min;
        #ifdef __arm__
            asm("vminm.f32 %[d], %[n], %[m]" : [d] "=t"(min) : [n] "t"(a), [m] "t"(b) :);
        #else 
            min = a < b ? a : b;
        #endif //__arm__
            return min;
    }
    // clamp for floats
    inline float fclamp(float in, float min, float max)
    {
        return fmin(fmax(in,min),max);
    }
    


}
#endif