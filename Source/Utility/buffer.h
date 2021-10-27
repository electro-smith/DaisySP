#pragma once
#include "daisysp.h"
#include <algorithm>

template <size_t max_size>
class Buffer
{
  public:
    Buffer() {}
    ~Buffer() {}

    /** Initializes the buffer. */
    void Init() { std::fill(&buff_[0], &buff_[max_size - 1], 0); }

    inline const float Read(size_t pos) const { return buff_[pos]; }

    /** Reads from a specified point in the delay line using linear interpolation */
    float ReadF(float pos)
    {
        float    a, b, frac;
        uint32_t i_idx = static_cast<uint32_t>(pos);
        frac           = pos - i_idx;
        a              = buff_[i_idx];
        b              = buff_[(i_idx + 1) % max_size];
        return a + (b - a) * frac;
    }


    inline void Write(size_t pos, float val) { buff_[pos] = val; }

  private:
    float buff_[max_size];
};
