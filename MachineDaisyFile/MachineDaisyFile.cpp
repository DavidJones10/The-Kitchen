#include "daisy_seed.h"
#include "daisysp.h"
#include "../EZ_DSP/EZ_DSP.h"

#define MAX_DELAY 96000
#define NUM_STEPS 8

using namespace daisy;
using namespace daisysp;

constexpr Pin BUTTONS     = seed::D15;
constexpr Pin SELECTOR    = seed::D16;
constexpr Pin TEMPO_MULT   = seed::D17;
constexpr Pin TEMPO       = seed::D18;
constexpr Pin TEMPO_MOD   = seed::D19;

static DaisySeed hw;
static AdcHandle adc;


enum drumModes
{
	KICK, SNARE, HAT, SYNTH_KICK
};

int drumMode = 0;
Switch selector, tempoModON;// selector: Drum sound changes when pressed, tempoModOn: holding down doubles or halves tempo
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

bool snares[8] = {false, false, false, false, false, false, false, false};
bool kicks[8] = {false, false, false, false, false, false, false, false};
bool hats[8] = {false, false, false, false, false, false, false, false};
bool synths[8] = {false, false, false, false, false, false, false, false};

int step=0;
bool patternIn = false;
bool tempoMode = true; //true = Double Time when timeMod pressed, false = half-time
float tempoKnobValue, onSwitchValue, tempoMult;
uint32_t debounceDelay = 100000; // 100000us 100ms
uint32_t lastDebounceTime = 0;

float delFeedback = .5f, delTime = .25f*48000, delWet = .5f;
//====================================================================================
// clears drum pattern 
void clearPattern()
{
	for (int i=0; i < NUM_STEPS; i++)
	{
		snares[i] = kicks[i] = hats[i] = synths[i] = false;
	}
	patternIn = false;
}
//====================================================================================
// gets button press for a specific button in the ladder
bool isButtonPressed(int buttonIdx)
{
	buttonValue = hw.adc.GetFloat(0);
	buttonValue = EZ_DSP::fmap(buttonValue, 0, 3300); // maps 0-1 value to milliamp values
	if (buttonValue < 100 && buttonIdx == 0)
		return true;
	if (buttonValue > 200 && buttonValue < 600 && buttonIdx == 1)
		return true;
	if (buttonValue > 600 && buttonValue < 1000 && buttonIdx == 2)
		return true;
	if (buttonValue > 1000 && buttonValue < 1400 && buttonIdx == 3)
		return true;
	if (buttonValue > 1400 && buttonValue < 1800 && buttonIdx == 4)
		return true;
	if (buttonValue > 1800 && buttonValue < 2200 && buttonIdx == 5)
		return true;
	if (buttonValue > 2200 && buttonValue < 2600 && buttonIdx == 6)
		return true;
	if (buttonValue > 2600 && buttonValue < 3000 && buttonIdx == 7)
		return true;
	else return false;
}
//====================================================================================
// gets button ladder presses and assigns them to boolean arrays 
void processButtons()
{
	if (System::GetUs() - lastDebounceTime > debounceDelay)
	{
		for (int i = 0; i < NUM_STEPS; i++)
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
				patternIn = true;
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
	hat.SetDecay(.4f);
	hat.SetTone(.5f);

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
	tempoModON.Debounce();
	if (selector.RisingEdge())
	{
		drumMode++;
		drumMode = drumMode % 4;
	}
	if (tempoModON.Pressed())
	{
		if (tempoMode)
			tempoMult = 2.f;
		else
			tempoMult = .5f;
	}
	else	
		tempoMult = 1.f;
	if (selector.TimeHeldMs() > 2000.f && tempoModON.TimeHeldMs() > 2000)
		clearPattern();

}
//====================================================================================
void processKnobAndSwitch()
{
	tempoKnobValue = hw.adc.GetFloat(1);
	onSwitchValue = hw.adc.GetFloat(2);
	// changes mode between sequencer and button mash
	if (onSwitchValue > .5f)
		tempoMode = true;
	else	
		tempoMode = false;

	//knob stuff
	float bpmValue = EZ_DSP::fmap(tempoKnobValue, 30.f, 250.f);
	float msValue = EZ_DSP::bpmToHz(bpmValue * tempoMult, NUM_STEPS); 
	tick.SetFreq(msValue);
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
	tempoModON.Init(TEMPO_MOD,hw.AudioCallbackRate());
	initDrums();
	AdcChannelConfig cfg[3];
	cfg[0].InitSingle(BUTTONS);// button array
	cfg[1].InitSingle(TEMPO);//knob
	cfg[2].InitSingle(TEMPO_MULT);// switch
	hw.adc.Init(cfg,3);
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
	bool t;
	t = tick.Process();
	
    // Process each drum sound individually and accumulate their outputs
    kickOut = kick.Process(kicks[step] && t);
    snareOut = snare.Process(snares[step] && t);
    hatOut = hat.Process(hats[step] && t);
    synthOut = synthKick.Process(synths[step] && t);

    // Sum the outputs of the drum sounds with their respective gain controls
    float kickVolume = 1.f; // Adjust the volume as needed
    float snareVolume = 0.5f;
    float hatVolume = 5.5f;
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
	processKnobAndSwitch();
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
		//hw.SetLed(patternIn);
		hw.SetLed(step%2==0);
		// simple test to see if drum selector button works, if button pressed, on board led lights up
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
