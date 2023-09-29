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

//====================================================================================
Metro tick;
AnalogBassDrum kick;
HiHat<SquareNoise, LinearVCA, true> hat;
SyntheticBassDrum synthKick;
SyntheticSnareDrum snare;
AdEnv kickEnv, snareEnv, hatEnv, synthEnv;
//EZ_DSP::DelayLine<float, MAX_DELAY> DSY_SDRAM_BSS del;

//EZ_DSP::kickDrum ourKick;

bool snares[4] = {false, false, false, false};
bool kicks[4] = {false, false, false, false};
bool hats[4] = {false, false, false, false};
bool synths[4] = {false, false, false, false};

int step=0;
uint32_t debounceDelay = 100000; // 100000us 100ms
uint32_t lastDebounceTime = 0;

float delFeedback = .5f, delTime = .25f*48000, delWet = .5f;
//====================================================================================
// maps input of 0-1 to number between min and max
float myfmap(float in, float min, float max)
{
	return EZ_DSP::fclamp(min + in * (max - min), min, max);
}

//====================================================================================
// gets button press for a specific button in the ladder
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
// gets button ladder presses and assigns them to boolean arrays 
void processButtons()
{
	if (System::GetUs() - lastDebounceTime > debounceDelay)
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
		lastDebounceTime = System::GetUs();	
	}
}
//====================================================================================
// sets drum parameters
void setDrumParams()
{
	kick.SetFreq(100.f);


	hat.SetAccent(.5f);
	hat.SetDecay(.1f);
	hat.SetTone(.1f);

	synthKick.SetAccent(.5f);
    synthKick.SetDirtiness(.1f);
    synthKick.SetDecay(.7f);
	synthKick.SetFreq(150.f);
}
//====================================================================================
// processes selector button
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
// initializes drums
void initDrums()
{
	float fs = hw.AudioSampleRate();
	tick.Init(4.f, fs);
	//ourKick.init(fs, 0);
	kick.Init(fs);
	snare.Init(fs);
	hat.Init(fs);
	synthKick.Init(fs);
}
//====================================================================================
// initializes all buttons and drum classes
void initControls()
{
	selector.Init(SELECTOR,hw.AudioCallbackRate());
	initDrums();
	AdcChannelConfig cfg;
	cfg.InitSingle(BUTTONS);
	hw.adc.Init(&cfg,1);
	hw.adc.Start();
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
// processes drum audio with sequencer controls
float processSequencer()
{	
	float totalOut = 0.f;
	float kickOut = 0.0f;
    float snareOut = 0.0f;
    float hatOut = 0.0f;
    float synthOut = 0.0f;

	bool t = tick.Process();
    // Process each drum sound individually and accumulate their outputs
    kickOut = kick.Process(kicks[step] && t);
    snareOut = snare.Process(snares[step] && t);
    hatOut = hat.Process(hats[step] && t);
    synthOut = synthKick.Process(synths[step] && t);

    // Sum the outputs of the drum sounds with their respective gain controls
    float kickVolume = 1.99f; // Adjust the volume as needed
    float snareVolume = 0.5f;
    float hatVolume = 0.5f;
    float synthVolume = 0.5f;

    totalOut = kickOut * kickVolume + snareOut * snareVolume + hatOut * hatVolume + synthOut * synthVolume;
	if (t)
	{
		step++;
		step = step % NUM_STEPS;
		setDrumParams();
	}
	return totalOut;
	
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
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	processButtons();// processes button voltage ladder
	digitalButtons();// processes drum selector button
	for (size_t i = 0; i < size; i++)
	{		
		out[0][i] = out[1][i] = processSequencer();
	}
}
//====================================================================================
int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_32KHZ); // set to 32khz because 48khz is too fast for all of the drum classes at once
	//hw.StartLog(true);
	hw.SetLed(false);
	//del.setDelay(delTime);
	initControls();
	hw.StartAudio(AudioCallback);
	for (;;)
	{
		// simple test to see if drum selector button works, if button pressed, on board led lights up
		if (selector.Pressed())
			hw.SetLed(true);
		else
			hw.SetLed(false);
		//System::Delay(100);
		//processButtons();
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
		/**/
	}
}
