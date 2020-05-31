/* ------------------------------------------------------------
author: "Grame"
copyright: "(c)GRAME 2009"
license: "BSD"
name: "Noise"
version: "1.1"
Code generated with Faust 2.23.4 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef __mydsp_H__
#define __mydsp_H__

/************************************************************************
 IMPORTANT NOTE : this file contains two clearly delimited sections :
 the ARCHITECTURE section (in two parts) and the USER section. Each section
 is governed by its own copyright and license. Please check individually
 each section for license and copyright information.
 *************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 
 ************************************************************************
 ************************************************************************/

#include "daisysp.h"
#include "daisy_seed.h"

#include "faust/gui/UI.h"
#include "faust/dsp/dsp.h"

#ifdef MIDICTRL
#include "faust/midi/daisy-midi.h"
#endif

using namespace daisysp;
using namespace daisy;

struct Meta
{
    virtual ~Meta(){};
    virtual void declare(const char* key, const char* value) = 0;
};

/******************************************************************************
 *******************************************************************************
 
 VECTOR INTRINSICS
 
 *******************************************************************************
 *******************************************************************************/


/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <algorithm>
#include <cmath>


#ifndef FAUSTCLASS
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp
{
  private:
    FAUSTFLOAT fHslider0;
    int        iRec0[2];
    FAUSTFLOAT fHslider1;
    int        fSampleRate;

  public:
    void metadata(Meta* m)
    {
        m->declare("author", "Grame");
        m->declare("copyright", "(c)GRAME 2009");
        m->declare("filename", "noise.dsp");
        m->declare("license", "BSD");
        m->declare("name", "Noise");
        m->declare("version", "1.1");
    }

    virtual int getNumInputs() { return 0; }
    virtual int getNumOutputs() { return 2; }
    virtual int getInputRate(int channel)
    {
        int rate;
        switch((channel))
        {
            default:
            {
                rate = -1;
                break;
            }
        }
        return rate;
    }
    virtual int getOutputRate(int channel)
    {
        int rate;
        switch((channel))
        {
            case 0:
            {
                rate = 1;
                break;
            }
            case 1:
            {
                rate = 1;
                break;
            }
            default:
            {
                rate = -1;
                break;
            }
        }
        return rate;
    }

    static void  classInit(int sample_rate) {}
    virtual void instanceConstants(int sample_rate)
    {
        fSampleRate = sample_rate;
    }

    virtual void instanceResetUserInterface()
    {
        fHslider0 = FAUSTFLOAT(0.5f);
        fHslider1 = FAUSTFLOAT(0.5f);
    }

    virtual void instanceClear()
    {
        for(int l0 = 0; (l0 < 2); l0 = (l0 + 1))
        {
            iRec0[l0] = 0;
        }
    }

    virtual void init(int sample_rate)
    {
        classInit(sample_rate);
        instanceInit(sample_rate);
    }
    virtual void instanceInit(int sample_rate)
    {
        instanceConstants(sample_rate);
        instanceResetUserInterface();
        instanceClear();
    }

    virtual mydsp* clone() { return new mydsp(); }
    virtual int    getSampleRate() { return fSampleRate; }
    virtual void   buildUserInterface(UI* ui_interface)
    {
        ui_interface->openVerticalBox("Noise");
        ui_interface->addHorizontalSlider(
            "vol1", &fHslider0, 0.5f, 0.0f, 1.0f, 0.00999999978f);
        ui_interface->addHorizontalSlider(
            "vol2", &fHslider1, 0.5f, 0.0f, 1.0f, 0.00999999978f);
        ui_interface->closeBox();
    }

    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        FAUSTFLOAT* output0 = outputs[0];
        FAUSTFLOAT* output1 = outputs[1];
        float       fSlow0  = (4.65661287e-10f * float(fHslider0));
        float       fSlow1  = (4.65661287e-10f * float(fHslider1));
        for(int i = 0; (i < count); i = (i + 1))
        {
            iRec0[0]     = ((1103515245 * iRec0[1]) + 12345);
            float fTemp0 = float(iRec0[0]);
            output0[i]   = FAUSTFLOAT((fSlow0 * fTemp0));
            output1[i]   = FAUSTFLOAT((fSlow1 * fTemp0));
            iRec0[1]     = iRec0[0];
        }
    }
};

/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/

FAUSTFLOAT* finputs[2];
FAUSTFLOAT* foutputs[2];

mydsp DSP;

#define MY_BUFFER_SIZE 8

static DaisySeed seed;

static void AudioCallback(float* in, float* out, size_t size)
{
    // Deinterleave
    for(size_t frame = 0; frame < size; frame++)
    {
        finputs[0][frame] = in[frame * 2];
        finputs[1][frame] = in[frame * 2 + 1];
    }

    // Faust processing
    DSP.compute(size, finputs, foutputs);

    // Interleave
    for(size_t frame = 0; frame < size; frame++)
    {
        out[frame * 2]     = foutputs[0][frame];
        out[frame * 2 + 1] = foutputs[1][frame];
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();

    // set buffer-size
    seed.SetAudioBlockSize(MY_BUFFER_SIZE);

    // allocate deinterleaved buffers
    finputs[0] = new FAUSTFLOAT[MY_BUFFER_SIZE];
    finputs[1] = new FAUSTFLOAT[MY_BUFFER_SIZE];

    foutputs[0] = new FAUSTFLOAT[MY_BUFFER_SIZE];
    foutputs[1] = new FAUSTFLOAT[MY_BUFFER_SIZE];

    // inti Faust DSP
    DSP.init(sample_rate);

#ifdef MIDICTRL
    daisy_midi midi_handler;
#endif


    // start callback
    seed.StartAudio(AudioCallback);


#ifdef MIDICTRL
    midi_handler.startMidi();
    ;
#endif

    while(1)
    {
#ifdef MIDICTRL
        midi_handler.processMidi();
#endif
    }
}

/********************END ARCHITECTURE SECTION (part 2/2)****************/

#endif
