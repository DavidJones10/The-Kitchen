#ifndef EZ_DSP_TREMOLO_H
#define EZ_DSP_TREMOLO_H

namespace EZ_DSP
{

class Tremolo
{
public:
    Tremolo(){}
    ~Tremolo(){}

    void init(float sample_rate);
    
    float process(float input);

    void setRate(float rate);

    void setDepth(float depth);

    void setWaveform(int waveform);

private:
    float frequency;
    float lfoDepth;
    int waveType;
};
} // namespace EZ_DSP
#endif