#pragma once
#ifndef DSY_FM2_H
#define DSY_FM2_H

#include <stdint.h>
#include "oscillator.h"
#ifdef __cplusplus

namespace daisysp
{
/** Simple 2 operator FM synth voice.
     
    Date: November, 2020

    Author: Ben Sergentanis
*/

class Fm2
{
  public:
    Fm2() {}
    ~Fm2() {}
	
    /** Initializes the FM2 module.
        \param samplerate - The sample rate of the audio engine being run. 
    */
    void Init(float samplerate);


    /**  Returns the next sample
    */
    float Process();
	
	/** Carrier freq. setter
	    \param freq Carrier frequency in Hz
	*/
	void SetCarrierFrequency(float freq);

	/** Modulator freq. setter
		\param freq Modulator frequency in Hz
	*/
	void SetModulatorFrequency(float freq);

        /** Set modulator freq. relative to carrier
		\param ratio New modulator freq = carrier freq. * ratio
	*/
	void SetRatio(float ratio);
	
	/** Setter for carrier waveform
		\param wave New waveform
	*/
	void SetCarrierWaveform(uint8_t wave);

	/** Setter for modulator waveform.
		\param wave New waveform
	*/
	void SetModulatorWaveform(uint8_t wave);

  /** Index setter
      \param FM depth, in Hz.
  */
  void SetIndex(float index);

  /** Returns the current FM index. */
  float GetIndex();

  /** Resets both oscillators */
  void Reset();
  
  private:
  Oscillator mod_, car_;
  float car_freq_, idx_;
  
};
} // namespace daisysp
#endif
#endif
