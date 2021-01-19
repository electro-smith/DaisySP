#pragma once
#ifndef DAISY_JITTER
#define DAISY_JITTER

namespace daisysp
{
/** Randomly segmented line generator \n 
    Originally extracted from csound by Paul Batchelor.\n 
    Ported by Ben Sergentanis, June 2020

    @author Gabriel Maldonado

    @year 1998

    Location: Opcodes/uggab.c (csound)
*/
class Jitter
{
  public:
    Jitter() {}
    ~Jitter() {}

    /** Initializes Jitter module
    \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get next floating point jitter sample */
    float Process();

    /** Set the minimum speed of the jitter engine.
    \param cps_min Number of new jitters per second
    */
    void SetCpsMin(float cps_min);

    /** Set the maximum speed of the jitter engine.
    \param cps_max Maximum number of jitters per second.
    */
    void SetCpsMax(float cps_max);

    /** Set the amplitude of the jitter. Jitters fall from -amp to +amp
    \param amp Jitter amplitude
    */
    void SetAmp(float amp);

  private:
    float   amp_, cps_min_, cps_max_, cps_, sample_rate_;
    int32_t phs_;
    bool    init_flag_;
    float   num1_, num2_, dfd_max_;
    float   randGab();
    float   biRandGab();
    void    Reset();
};
} // namespace daisysp

#endif
