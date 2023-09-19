#include "kickDrum.h"
#include "oscillator.cpp"

kickDrum::kickDrum(double sampleRate,int waveType) {
    this->sampleRate = sampleRate;
    waveformSize = 44100; 
    waveform = new double[waveformSize];

    volumeDecay = 0.0;
    volumeSustain = 0.0;
    pitchDecay = 0.0;
    pitchStart = 0.0;
    pitchEnd = 0.0;
    lfoFrequency = 1.0; 
    lfoPhase = 0.0;

}


kickDrum::~kickDrum() {
    delete[] waveform;
}

void kickDrum::setVolumeEnvelope(double decay, double sustain) {
    volumeDecay = decay;
    volumeSustain = sustain;
}

void kickDrum::setPitchEnvelope(double decay, double pitchStart, double pitchEnd) {
    pitchDecay = decay;
    this->pitchStart = pitchStart;
    this->pitchEnd = pitchEnd;
}

void kickDrum::selectWaveform(int waveType) {
   switch (waveType) {
        case 0: // Sine wave
            generateSineWave(this->waveform, waveformSize);
            break;
        case 1: // Square wave
            generateSquareWave(this->waveform, waveformSize);
            break;
        default:
            // Handle unsupported waveform type
            break;
    }
}

double kickDrum::calculateEnvelope(double time, double decay, double sustain) {
    if (time < 0.0) {
        return 0.0;  // Envelope not started yet
    } else if (time < decay) {
        return 1.0 - (1.0 - sustain) * (1.0 - exp(-time / decay));
    } else {
        return sustain;
    }
}

void kickDrum::generateAudio(double* buffer, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        // Calculate the volume envelope value at this time
        double volumeEnvelope = calculateEnvelope(i / sampleRate, volumeDecay, volumeSustain);

        // Calculate the pitch envelope value at this time
        double pitchEnvelope = calculateEnvelope(i / sampleRate, pitchDecay, 1.0);  // Sustain level of 1.0

        // Calculate the current pitch based on the pitch envelope
        double currentPitch = pitchStart + (pitchEnd - pitchStart) * pitchEnvelope;

        // Calculate the LFO value (e.g., using a sine wave)
        double lfoValue = sin(2.0 * M_PI * lfoFrequency * i / sampleRate);

        // Adjust modulationAmount to control the depth of modulation
        double modulatedPitch = currentPitch + modulationAmount * lfoValue;

        // Calculate the angular frequency for the oscillator based on modulatedPitch
        double angularFrequency = 2.0 * M_PI * modulatedPitch;

        // Initialize the phase of the oscillator
        double phase = 0.0;

        // Calculate the current sample value by modulating the waveform with volume and pitch
        double sampleValue = 0.0;  // Initialize the sample value

        for (int j = 0; j < numOscillatorCycles; ++j) {
            // Generate the sample by summing oscillators with different phases
            sampleValue += sin(phase);
            
            // Update the phase for the next sample
            phase += angularFrequency / sampleRate;
        }

        // Normalize the sample value
        sampleValue /= numOscillatorCycles;

        // Apply volume envelope to the sample
        sampleValue *= volumeEnvelope;

        // Store the sample in the buffer
        buffer[i] = sampleValue;
    }
}



