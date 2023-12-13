//#include <stdio.h>
//#include <time.h>

//#include "../DaisyExamples/libDaisy/src/per/tim.h"
//#include "../DaisyExamples/libDaisy/src/util/hal_map.h"
//#include "../DaisyExamples/libDaisy/src/sys/system.h"
class sequencer
{
    int numSteps = 8;
    int step;

    bool kick [8];
    bool snare[8];
    bool hat[8];
    bool crash[8];
    bool trig[4];

    bool trigSound()
    {
        //if (CLOCKS_PER_SEC % clock() == 0)
        if (1000 % (GetTick() / (GetFreq() / 1000000)) == 0)
        {
            //step = numSteps % (clock() / CLOCKS_PER_SEC);
            step = (GetTick() / (GetFreq() / 1000000)) / 1000;

            trig[0] = kick[step];
            trig[1] = snare[step];
            trig[2] = hat[step];
            trig[3] = crash[step];
        }
    }

    bool trigReset()
    {
        for(int i = 0; i < sizeof(trig); i++)
        {
            trig[i] = 0;
        }
    }
};
