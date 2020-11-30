#pragma once
#ifndef DSY_FM2_H
#define DSY_FM2_H

#include <stdint.h>
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
        \param sample_rate - The sample rate of the audio engine being run. 
    */
    void Init(float sample_rate);


    /**  Returns the next sample
    */
    float Process();
	
	/** Carrier freq. setter
		\param freq Carrier frequency in Hz
	*/
	void SetCarrierFrequency(float freq);

	/** Carrier freq. getter */
	float GetCarrierFrequency();

	/** Modulator freq. setter
		\param freq Modulator frequency in Hz
	*/
	void SetModulatorFrequency(float freq);
	
	/** Modulator freq. getter */
	float GetModulatorFrequency();
	
	/** Set modulator freq. relative to carrier
		\param ratio New modulator freq = carrier freq. * ratio
	*/
	void SetRatio(float ratio);
	
	/** Setter for carrier waveform
		\param wave New waveform
	*/
	void SetCarrierWaveform(Oscillator::Waveform wave);

	/** Setter for modulator waveform.
		\param wave New waveform
	*/
	void SetModulatorWaveform(Oscillator::Waveform wave);
	
	
	

  private:
};
} // namespace daisysp
#endif
#endif
