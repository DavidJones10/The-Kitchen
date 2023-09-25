#include <cmath>

void generateSineWave(float* waveform, int waveformSize) 
{
    for (int i = 0; i < waveformSize; ++i) 
    {
        float phase = 2.0 * M_PI * i / waveformSize;
        waveform[i] = sinf(phase);
    }
}

void generateSquareWave(float* waveform, int waveformSize) 
{
    for (int i = 0; i < waveformSize; ++i) 
    {
        if (i < waveformSize / 2) 
            waveform[i] = 1.0;
        else 
            waveform[i] = -1.0;
    }
}
