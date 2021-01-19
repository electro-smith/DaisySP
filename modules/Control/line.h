#pragma once
#ifndef LINE_H
#define LINE_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** creates a Line segment signal
    */
class Line
{
  public:
    Line() {}
    ~Line() {}
    /** Initializes Line module.
    */
    void Init(float sample_rate);

    /** Processes Line segment. Returns one sample.
        value of finished will be updated to a 1, upon completion of the Line's trajectory.
    */
    float Process(uint8_t *finished);

    /** Begin creation of Line. 
        \param start - beginning value
        \param end - ending value
        \param dur - duration in seconds of Line segment
    */
    void Start(float start, float end, float dur);

  private:
    float   start_, end_, dur_;
    float   inc_, val_, sample_rate_;
    uint8_t finished_;
};
} // namespace daisysp
#endif
#endif
