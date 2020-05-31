#pragma once
#ifndef DSY_DELAY_H
#define DSY_DELAY_H
#include <stdlib.h>
#include <stdint.h>
namespace daisysp
{

/** Simple Delay line.
November 2019
Converted to Template December 2019
declaration example: (1 second of floats)
```C++
DelayLine<float, SAMPLE_RATE> del;
```
By: shensley
*/
template<typename T, size_t max_size>
class DelayLine
{
    public:
    DelayLine() { }
    ~DelayLine() { }

/** initializes the delay line by clearing the values within, and setting delay to 1 sample.
*/
    void Init()
    {
        Reset();
    }

/** clears buffer, sets write ptr to 0, and delay to 1 sample.
*/
    void Reset() {
        for (size_t i = 0; i < max_size; i++)
        {
            line_[i] = T(0);
        }
        write_ptr_ = 0;
        delay_ = 1;
    }

/** sets the delay time in samples
If a float is passed in, a fractional component will be calculated for interpolating the delay line.
*/
    inline void SetDelay(size_t delay)
    {
        frac_ = 0.0f;
        delay_ = delay < max_size ? delay : max_size - 1;
    }

    inline void SetDelay(float delay)
    {
        int32_t int_delay = static_cast<int32_t>(delay);
        frac_ = delay - static_cast<float>(int_delay);
        delay_ = static_cast<size_t>(int_delay) < max_size ? int_delay : max_size - 1;
    }

/** writes the sample of type T to the delay line, and advances the write ptr
*/
    inline void Write(const T sample) 
    {
        line_[write_ptr_] = sample;
        write_ptr_ = (write_ptr_ - 1 + max_size) % max_size;
    }

/** returns the next sample of type T in the delay line, interpolated if necessary.
*/
    inline const T Read() const
    {
        T a = line_[(write_ptr_ + delay_) % max_size];
        T b = line_[(write_ptr_ + delay_ + 1) % max_size];
        return a + (b - a) * frac_;
    }

    private:
    float frac_;
    size_t write_ptr_;
    size_t delay_;
    T line_[max_size];

};
} // namespace daisysp
#endif
