/*
Copyright (c) 2020 Electrosmith, Corp, Brian Tice, Tom Erbe

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once

namespace daisysp
{
/** Second Order All Pass Filter

This is from Tom Erbe's synth notes. The filter
can be configured to be a band pass or band reject

Ported by: Brian Tice
*/
class Soap
{
  public:
    Soap() {}
    ~Soap() {}

    /** Initializes the filter
            float sample_rate - sample rate of the audio engine being run, and the frequency that the Process function will be called.
        */
    void Init(float sample_rate);

    /**
            Process the input signal, updating all of the outputs
        */
    void Process(float in);

    /**
            Sets the center frequency of the filter.
        */
    void SetCenterFreq(float f);

    /**
            Sets the low frequency threshold of the filter.
        */
    void SetFilterBandwidth(float b);

    /** Bandpass output
            \return bandpass output of the filter
        */
    inline float Bandpass() { return out_bandpass_; }

    /** Bandreject output
            \return bandreject output of the filter
        */
    inline float Bandreject() { return out_bandreject_; }

  private:
    float soap_center_freq_;
    float soap_bandwidth_;
    float in_0_;
    float din_1_;
    float din_2_;
    float dout_1_;
    float dout_2_;
    float all_output_;
    float out_bandpass_;
    float out_bandreject_;
    float sr_;
};

} // namespace daisysp
