#include "daisy_seed.h"
#include "daisysp.h"
#include "../EZ_DSP/EZ_DSP.h"

using namespace daisy;
using namespace daisysp;

constexpr Pin BUTTONS = seed::D15;

DaisySeed hw;
AnalogControl buttons;
AnalogBassDrum kick;
AnalogSnareDrum snare;
HiHat<SquareNoise, LinearVCA, true> hat;
SyntheticBassDrumClick synthKick;

float myfmap(float in, float min, float max)
{
	return EZ_DSP::fclamp(min + in * (max - min), min, max);
}
// TODO: this is NOT working, find out why
void buttonsToAudio(float &outL, float &outR)
{
	float buttonValue = buttons.Process();
	buttonValue = myfmap(buttonValue, 0, 3300); // maps 0-1 value to milliamp values
	if (buttonValue < 100)
	{
		outL += kick.Process(true);
		outR += kick.Process(true);

	}
	else if (buttonValue > 200 && buttonValue < 500)
	{
		outL += snare.Process(true);
		outR += snare.Process(true);
	}
	else if (buttonValue > 1000 && buttonValue < 1750)
	{
		outL += hat.Process(true);
		outR += hat.Process(true);
	}
	else if (buttonValue > 1900 && buttonValue < 2500)
	{
		outL += synthKick.Process(true);
		outR += synthKick.Process(true);
	}
	else 
	{
		outL=outR=0;
	}
}

void initDrums()
{
	float fs = hw.AudioSampleRate();
	kick.Init(fs);
	snare.Init(fs);
	hat.Init(fs);
	synthKick.Init(fs);
}

void initControls()
{
	AdcChannelConfig cfg;
	cfg.InitSingle(BUTTONS);
	hw.adc.Init(&cfg,1);
	hw.adc.Start();
	initDrums();
}


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		buttonsToAudio(out[0][i], out[1][i]);
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	initControls();
	hw.SetLed(true);
	hw.StartAudio(AudioCallback);
	while(1) {}
}

