#pragma once

class kickDrum 
{
public:
    kickDrum(float sampleRate, int waveType);
    ~kickDrum();

    void setVolumeEnvelope(float decay, float sustain);
    void setPitchEnvelope(float decay, float pitchStart, float pitchEnd);
    void selectWaveform(int waveform);
    void setModulationAmount(float amount);
    void setNumOscillatorCycles(int cycles);

    float generateAudio(float in, int index);

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

    float calculateEnvelope(float time, float decay, float sustain);
};
