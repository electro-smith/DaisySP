
#pragma once
#ifndef DSY_DCBLOCK_H
#define DSY_DCBLOCK_H
#ifdef __cplusplus

namespace daisysp
{
/** Removes DC component of a signal
    */
class DcBlock
{
  public:
    DcBlock(){};
    ~DcBlock(){};

    /** Initializes DcBlock module
    */
    void Init(float sample_rate);

    /** performs DcBlock Process 
    */
    float Process(float in);

  private:
    float input_, output_, gain_;
};
} // namespace daisysp
#endif
#endif
