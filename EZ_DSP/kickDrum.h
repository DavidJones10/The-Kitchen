#pragma once

class kickDrum {
public:
    kickDrum(double sampleRate, int waveType);
    ~kickDrum();

    void setVolumeEnvelope(double decay, double sustain);
    void setPitchEnvelope(double decay, double pitchStart, double pitchEnd);
    void selectWaveform(int waveform);
    void setModulationAmount(double amount);
    void setNumOscillatorCycles(int cycles);

    void generateAudio(double* buffer, int numSamples);

private:
    int waveType;
    double sampleRate;
    double* waveform;
    int waveformSize;
    double volumeDecay;
    double volumeSustain;
    double pitchDecay;
    double pitchStart;
    double pitchEnd;
    double lfoFrequency;
    double lfoPhase;
    double modulationAmount;
    int numOscillatorCycles;

    double calculateEnvelope(double time, double decay, double sustain);
};
