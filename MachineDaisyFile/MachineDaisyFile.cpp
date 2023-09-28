#include "daisy_seed.h"
#include "daisysp.h"
#include "../EZ_DSP/EZ_DSP.h"


#define MAX_DELAY 96000
#define NUM_STEPS 4

using namespace daisy;
using namespace daisysp;

constexpr Pin BUTTONS = seed::D15;
constexpr Pin SELECTOR = seed::D16;

static DaisySeed hw;
static AdcHandle adc;


enum drumModes
{
	KICK, SNARE, HAT, SYNTH_KICK
};

int drumMode = 0;
Switch selector;
float buttonValue;


Metro tick;
AnalogBassDrum kick;
AnalogSnareDrum snare;
HiHat<SquareNoise, LinearVCA, true> hat;
SyntheticBassDrum synthKick;
AdEnv kickEnv, snareEnv, hatEnv, synthEnv;
EZ_DSP::DelayLine<float, MAX_DELAY> DSY_SDRAM_BSS del;

bool snares[4] = {false, false, false, false};
bool kicks[4] = {false, false, false, false};
bool hats[4] = {false, false, false, false};
bool synths[4] = {false, false, false, false};

int step=0;

float delFeedback = .5f, delTime = .25f*48000, delWet = .5f;
bool kickOn=false, snareOn=false, hatOn=false, synthOn=false;

//====================================================================================
float myfmap(float in, float min, float max)
{
	return EZ_DSP::fclamp(min + in * (max - min), min, max);
}

//====================================================================================
bool isButtonPressed(int buttonIdx)
{
	buttonValue = hw.adc.GetFloat(0);
	buttonValue = myfmap(buttonValue, 0, 3300); // maps 0-1 value to milliamp values
	if (buttonValue < 1850 && buttonIdx == 0)
		return true;
	if (buttonValue > 1850 && buttonValue < 2000 && buttonIdx == 1)
		return true;
	if (buttonValue > 2000 && buttonValue < 2900 && buttonIdx == 2)
		return true;
	if (buttonValue > 2900 && buttonValue < 3100 && buttonIdx == 3)
		return true;
	else return false;
}
//====================================================================================
void processButtons()
{
	
	for (int i = 0; i < 4; i++)
    {
        if (isButtonPressed(i))
        {
            switch (drumMode)
            {
                case KICK:
                    // Activate the kick drum note in the sequence
                    kicks[i] = !kicks[i];
                    break;
                case SNARE:
                    // Activate the snare drum note in the sequence
                    snares[i] = !snares[i];
                    break;
                case HAT:
                    // Activate the hi-hat note in the sequence
                    hats[i] = !hats[i];
                    break;
                case SYNTH_KICK:
                    // Activate the synthetic kick drum note in the sequence
                    synths[i] = !synths[i];
                    break;
                // Add more cases for other drum modes if needed
            }
        }
    }
}
//====================================================================================
void setDrumParams()
{
	kick.SetTone(.7f );
    kick.SetDecay(.4f);
    kick.SetSelfFmAmount(.2f);

	snare.SetAccent(.5f);
	snare.SetDecay(.5f);
	snare.SetTone(1.f);
	snare.SetSnappy(.6);

	hat.SetAccent(.5f);
	hat.SetDecay(.1f);
	hat.SetTone(.1f);

	synthKick.SetAccent(.5f);
    synthKick.SetDirtiness(.1f);
    synthKick.SetDecay(.7f);
	synthKick.SetFreq(150.f);
}
//====================================================================================
void digitalButtons()
{
	selector.Debounce();
	if (selector.RisingEdge())
	{
		drumMode++;
		drumMode = drumMode % 4;
	}
}
//====================================================================================
void sequencer(float &outL, float &outR)
{	
	
	digitalButtons();
	bool t = tick.Process();
	outL = kick.Process(kicks[step]&&t) + snare.Process(snares[step]&&t) + hat.Process(snares[step]&&t) + synthKick.Process(synths[step]&&t);
	outR = kick.Process(kicks[step]&&t) + snare.Process(snares[step]&&t) + hat.Process(snares[step]&&t) + synthKick.Process(synths[step]&&t);
	if (t)
	{
		step++;
		step = step % NUM_STEPS;
	}
	
	/*	
	float delOut, fback;
	
	delOut = del.read();
	fback = (delOut * delFeedback) + totalOut;
	del.write(fback);
	outL += totalOut*(1-delWet) + delOut*delWet;
	outR += totalOut*(1-delWet) + delOut*delWet;
	*/
}
//====================================================================================
void initDrums()
{
	float fs = hw.AudioSampleRate();
	kick.Init(fs);
	snare.Init(fs);
	hat.Init(fs);
	synthKick.Init(fs);
	tick.Init(4.f, hw.AudioSampleRate());
}
//====================================================================================
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
//====================================================================================
void initControls()
{
	AdcChannelConfig cfg;
	cfg.InitSingle(BUTTONS);
	hw.adc.Init(&cfg,1);
	hw.adc.Start();
	selector.Init(SELECTOR,1000);
	initDrums();
	//initEnvs();
}
/*
void ledMatrix()
{
	for (int i=0; i < 4; i++)
	{
		setled(0, i, (int)kicks[i]);
		setled(1, i, (int)snares[i]);
		setled(2, i, (int)hats[i]);
		setled(3, i, (int)synths[i]);
	}
}
*/
//====================================================================================
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	
	for (size_t i = 0; i < size; i++)
	{
		sequencer(out[0][i], out[1][i]);
	}
}
//====================================================================================
int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	//hw.StartLog(true);
	initControls();
	hw.SetLed(true);
	del.setDelay(delTime);

	hw.StartAudio(AudioCallback);
	while(1) 
	{
		processButtons();
		setDrumParams();
		//ledMatrix();
		/*
		System::Delay(100);
		hw.PrintLine(" Button Value: " FLT_FMT3, FLT_VAR3(buttonValue));
		hw.PrintLine("Kick bool Array %d %d %d %d", kicks[0], kicks[1], kicks[2], kicks[3]);
		hw.PrintLine("Snare bool Array %d %d %d %d", snares[0], snares[1], snares[2], snares[3]);
		hw.PrintLine("Hat bool Array %d %d %d %d", hats[0], hats[1], hats[2], hats[3]);
		hw.PrintLine("Synth bool Array %d %d %d %d", synths[0], synths[1], synths[2], synths[3]);
		hw.PrintLine("Drum Mode: %d", drumMode);
		hw.PrintLine("Current Step %d", step);
		*/
	}
}
