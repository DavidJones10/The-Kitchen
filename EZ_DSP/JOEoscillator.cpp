#include <cmath>

void generateSineWave(double* waveform, int waveformSize) {
    for (int i = 0; i < waveformSize; ++i) {
        double phase = 2.0 * M_PI * i / waveformSize;
        waveform[i] = sin(phase);
    }
}

void generateSquareWave(double* waveform, int waveformSize) {
    for (int i = 0; i < waveformSize; ++i) {
        if (i < waveformSize / 2) {
            waveform[i] = 1.0;
        } else {
            waveform[i] = -1.0;
        }
    }
}
