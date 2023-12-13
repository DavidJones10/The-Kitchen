#pragma once
#ifndef EZ_DSP_KICKDRUM_H
#define EZ_DSP_KICKDRUM_H
namespace EZ_DSP
{
class kickDrum 
{
public:
    void init(float sample_rate, int waveType);
    void setVolumeEnvelope(float decay, float sustain);
    void setPitchEnvelope(float decay, float pitchStart, float pitchEnd);
    void selectWaveform(int waveform);
    void setModulationAmount(float amount);
    void setNumOscillatorCycles(int cycles);

    float generateAudio(bool trigger);

private:
    int waveType;
    float sampleRate;
    float* waveform;
    int waveformSize;
    float volumeDecay;
    float volumeSustain;
    float pitchDecay;
    float pitchStart;
    float pitchEnd;
    float lfoFrequency;
    float lfoPhase;
    float modulationAmount;
    int numOscillatorCycles;
    int count=0;
    bool counting = false;

    float calculateEnvelope(float time, float decay, float sustain);
};
}
#endif
