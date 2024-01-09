#pragma once 
#ifndef EZ_DSP_OVERDRIVE_H
#define EZ_DSP_OVERDRIVE_H
#include "dsp.h"

namespace EZ_DSP
{
class Overdrive
{
    public:

    private:
        float drive;
        float preGain;
        float postGain;
};
}
#endif