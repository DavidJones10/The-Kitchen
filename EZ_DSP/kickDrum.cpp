#include "kickDrum.h"
#include "JOEoscillator.cpp"

kickDrum::kickDrum(float sampleRate,int waveType) 
{
    this->sampleRate = sampleRate;
    waveformSize = 48000; 
    waveform = new float[waveformSize];

    volumeDecay = 0.0;
    volumeSustain = 0.0;
    pitchDecay = 0.0;
    pitchStart = 0.0;
    pitchEnd = 0.0;
    lfoFrequency = 1.0; 
    lfoPhase = 0.0;
}


kickDrum::~kickDrum() 
{
    delete[] waveform;
}

void kickDrum::setVolumeEnvelope(float decay, float sustain) 
{
    volumeDecay = decay;
    volumeSustain = sustain;
}

void kickDrum::setPitchEnvelope(float decay, float pitchStart, float pitchEnd) 
{
    pitchDecay = decay;
    this->pitchStart = pitchStart;
    this->pitchEnd = pitchEnd;
}

void kickDrum::selectWaveform(int waveType) 
{
   switch (waveType) 
   {
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

float kickDrum::calculateEnvelope(float time, float decay, float sustain) 
{
    if (time < 0.0) 
    {
        return 0.0;  // Envelope not started yet
    } 
    else if (time < decay) {
        return 1.0 - (1.0 - sustain) * (1.0 - exp(-time / decay));
    } 
    else 
    {
        return sustain;
    }
}

float kickDrum::generateAudio(float in, int index) 
{
        // Calculate the volume envelope value at this time
        float volumeEnvelope = calculateEnvelope(index / sampleRate, volumeDecay, volumeSustain);

        // Calculate the pitch envelope value at this time
        float pitchEnvelope = calculateEnvelope(index / sampleRate, pitchDecay, 1.0);  // Sustain level of 1.0

        // Calculate the current pitch based on the pitch envelope
        float currentPitch = pitchStart + (pitchEnd - pitchStart) * pitchEnvelope;

        // Calculate the LFO value (e.g., using a sine wave)
        float lfoValue = sin(2.0 * M_PI * lfoFrequency * index / sampleRate);

        // Adjust modulationAmount to control the depth of modulation
        float modulatedPitch = currentPitch + modulationAmount * lfoValue;

        // Calculate the angular frequency for the oscillator based on modulatedPitch
        float angularFrequency = 2.0 * M_PI * modulatedPitch;

        // Initialize the phase of the oscillator
        float phase = 0.0;

        // Calculate the current sample value by modulating the waveform with volume and pitch
        float sampleValue = 0.0;  // Initialize the sample value

        for (int j = 0; j < numOscillatorCycles; ++j) 
        {
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
        return sampleValue;
    
}



