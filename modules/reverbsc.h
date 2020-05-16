// # ReverbSc
// ## Description
//
// Stereo Reverb
//
// ## Credits
//
// Reverb SC:               Ported from csound/soundpipe
//
// Original author(s):	    Sean Costello, Istvan Varga
//
// Year:				    1999, 2005
//
// Ported to soundpipe by:  Paul Batchelor
// 
// Ported by:			    Stephen Hensley
//
//
#pragma once
#ifndef DSYSP_REVERBSC_H
#define DSYSP_REVERBSC_H

#define DSY_REVERBSC_MAX_SIZE 98936

namespace daisysp
{
typedef struct
{
    int    write_pos;
    int    buffer_size;
    int    read_pos;
    int    read_pos_frac;
    int    read_pos_frac_inc;
    int    dummy;
    int    seed_val;
    int    rand_line_cnt;
    float  filter_state;
    float *buf;
} ReverbScDl;

class ReverbSc
{
  public:
    ReverbSc() {}
    ~ReverbSc() {}
    // ### Init
    // Initializes the reverb module, and sets the sample_rate at which the Process function will be called.
	// 
    // Returns 0 if all good, or 1 if it runs out of delay times exceed maximum allowed.
	//
    // ~~~~
    int Init(float sample_rate);
    // ~~~~

    // ### Process
    // Process the input through the reverb, and updates values of out1, and out2 with the new processed signal.
    // ~~~~
    int Process(const float &in1, const float &in2, float *out1, float *out2);
    // ~~~~

    // ### SetFeedback
    // controls the reverb time. reverb tail becomes infinite when set to 1.0
    //
    // range: 0.0 to 1.0
    //
    // ~~~~
    inline void SetFeedback(const float &fb) { feedback_ = fb; }
    // ~~~~
    // ### SetLpFreq
    // controls the internal dampening filter's cutoff frequency.
    //
    // range: 0.0 to sample_rate / 2
    //
    // ~~~~
    inline void SetLpFreq(const float &freq) { lpfreq_ = freq; }
    // ~~~~

  private:
    void       NextRandomLineseg(ReverbScDl *lp, int n);
    int        InitDelayLine(ReverbScDl *lp, int n);
    float      feedback_, lpfreq_;
    float      i_sample_rate_, i_pitch_mod_, i_skip_init_;
    float      sample_rate_;
    float      damp_fact_;
    float      prv_lpfreq_;
    int        init_done_;
    ReverbScDl delay_lines_[8];
    float      aux_[DSY_REVERBSC_MAX_SIZE];
};


} // namespace daisysp
#endif
