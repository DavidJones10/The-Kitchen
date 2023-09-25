#include "daisy_seed.h"
#include "daisysp.h"
#include "../EZ_DSP/EZ_DSP.h"

#define MAX_DELAY 96000

using namespace daisy;
using namespace daisysp;

constexpr Pin BUTTONS = seed::D15;
float buttonValue;

static DaisySeed hw;
static AdcHandle adc;
AnalogBassDrum kick;
AnalogSnareDrum snare;
HiHat<SquareNoise, LinearVCA, true> hat;
SyntheticBassDrum synthKick;
AdEnv kickEnv, snareEnv, hatEnv, synthEnv;
EZ_DSP::DelayLine<float, MAX_DELAY> DSY_SDRAM_BSS del;

float delFeedback = .5f, delTime = .25f*48000, delWet = .5f;


float myfmap(float in, float min, float max)
{
	return EZ_DSP::fclamp(min + in * (max - min), min, max);
}
// TODO: this is NOT working, find out why
void buttonsToAudio(float &outL, float &outR)
{
	buttonValue = hw.adc.GetFloat(0);
	buttonValue = myfmap(buttonValue, 0, 3300); // maps 0-1 value to milliamp values
	float kickOut, snareOut, hatOut, synthOut, totalOut;
	
	if (buttonValue < 1850)
		kickEnv.Trigger();
	else if (buttonValue > 1850 && buttonValue < 2000)
		snareEnv.Trigger();
	else if (buttonValue > 2000 && buttonValue < 2900)
		hatEnv.Trigger();
	else if (buttonValue > 2900 && buttonValue < 3100)
		synthEnv.Trigger();
	
	kick.SetTone(.7f );
    kick.SetDecay(.4f);
    kick.SetSelfFmAmount(.2f);
	kickOut =  kick.Process(true);
	kickOut *= kickEnv.Process();

	snare.SetAccent(.5f);
	snare.SetDecay(.5f);
	snare.SetTone(1.f);
	snare.SetSnappy(.6);
	snareOut =  snare.Process(true);
	snareOut *= snareEnv.Process();

	hat.SetAccent(.5f);
	hat.SetDecay(.1f);
	hat.SetTone(.1f);
	hatOut =  hat.Process(true);
	hatOut *= hatEnv.Process();

	synthKick.SetAccent(.5f);
    synthKick.SetDirtiness(.1f);
    synthKick.SetDecay(.7f);
	synthKick.SetFreq(150.f);
	synthOut =  synthKick.Process(true);
	synthOut *= synthEnv.Process();

	totalOut = (kickOut + hatOut + snareOut + synthOut) *.25f;

	float delOut, fback;
	
	delOut = del.read();
	fback = (delOut * delFeedback) + totalOut;
	del.write(fback);
	outL += totalOut*(1-delWet) + delOut*delWet;
	outR += totalOut*(1-delWet) + delOut*delWet;
}

void initDrums()
{
	float fs = hw.AudioSampleRate();
	kick.Init(fs);
	snare.Init(fs);
	hat.Init(fs);
	synthKick.Init(fs);
}

void initEnvs()
{
	snareEnv.Init(hw.AudioSampleRate());
    snareEnv.SetTime(ADENV_SEG_ATTACK, .01);
    snareEnv.SetTime(ADENV_SEG_DECAY, .2);
    snareEnv.SetMax(1);
    snareEnv.SetMin(0);

	kickEnv.Init(hw.AudioSampleRate());
    kickEnv.SetTime(ADENV_SEG_ATTACK, .01);
    kickEnv.SetTime(ADENV_SEG_DECAY, .2);
    kickEnv.SetMax(1);
    kickEnv.SetMin(0);

	hatEnv.Init(hw.AudioSampleRate());
    hatEnv.SetTime(ADENV_SEG_ATTACK, .01);
    hatEnv.SetTime(ADENV_SEG_DECAY, .2);
    hatEnv.SetMax(1);
    hatEnv.SetMin(0);

	synthEnv.Init(hw.AudioSampleRate());
    synthEnv.SetTime(ADENV_SEG_ATTACK, .01);
    synthEnv.SetTime(ADENV_SEG_DECAY, .2);
    synthEnv.SetMax(1);
    synthEnv.SetMin(0);
}

void initControls()
{
	AdcChannelConfig cfg;
	cfg.InitSingle(BUTTONS);
	hw.adc.Init(&cfg,1);
	hw.adc.Start();
	initDrums();
	initEnvs();
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
	hw.StartLog(true);
	initControls();
	hw.SetLed(true);
	del.setDelay(delTime);
\
	hw.StartAudio(AudioCallback);
	while(1) 
	{
		System::Delay(200);
		hw.PrintLine(" Button Value: " FLT_FMT3, FLT_VAR3(buttonValue));
	}
}

