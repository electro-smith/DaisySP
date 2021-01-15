// Copyright 2014 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Base class for building reverbs.

#pragma once
#ifndef DSY_FX_ENGINE_H
#define DSY_FX_ENGINE_H

#include <algorithm>
#include <stdint.h>
#ifdef __cplusplus

#define JOIN(lhs, rhs) JOIN_1(lhs, rhs)
#define JOIN_1(lhs, rhs) JOIN_2(lhs, rhs)
#define JOIN_2(lhs, rhs) lhs##rhs

#define STATIC_ASSERT(expression, message)                                  \
    struct JOIN(__static_assertion_at_line_, __LINE__)                      \
    {                                                                       \
        impl::StaticAssertion<static_cast<bool>((expression))>              \
            JOIN(JOIN(JOIN(STATIC_ASSERTION_FAILED_AT_LINE_, __LINE__), _), \
                 message);                                                  \
    };

namespace impl
{
template <bool>
struct StaticAssertion;

template <>
struct StaticAssertion<true>
{
}; // StaticAssertion<true>

template <int i>
struct StaticAssertionTest
{
}; // StaticAssertionTest<int>

} // namespace impl

namespace daisysp
{
#define TAIL , -1

enum Format
{
    FORMAT_12_BIT,
    FORMAT_16_BIT,
    FORMAT_32_BIT
};

enum LFOIndex
{
    LFO_1,
    LFO_2
};

static inline int32_t Clip16(int32_t x)
{
    if(x < -32768)
    {
        return -32768;
    }
    else if(x > 32767)
    {
        return 32767;
    }
    else
    {
        return x;
    }
}

template <Format format>
struct DataType
{
};

template <>
struct DataType<FORMAT_12_BIT>
{
    typedef uint16_t T;

    static inline float Decompress(T value)
    {
        return static_cast<float>(static_cast<int16_t>(value)) / 4096.0f;
    }

    static inline T Compress(float value)
    {
        return static_cast<uint16_t>(
            Clip16(static_cast<int32_t>(value * 4096.0f)));
    }
};

template <>
struct DataType<FORMAT_16_BIT>
{
    typedef uint16_t T;

    static inline float Decompress(T value)
    {
        return static_cast<float>(static_cast<int16_t>(value)) / 32768.0f;
    }

    static inline T Compress(float value)
    {
        return static_cast<uint16_t>(
            Clip16(static_cast<int32_t>(value * 32768.0f)));
    }
};

template <>
struct DataType<FORMAT_32_BIT>
{
    typedef float T;

    static inline float Decompress(T value)
    {
        return value;
        ;
    }

    static inline T Compress(float value) { return value; }
};

template <size_t size, Format format = FORMAT_12_BIT>
class FxEngine
{
  public:
    typedef typename DataType<format>::T T;
    FxEngine() {}
    ~FxEngine() {}

    void Init(T* buffer)
    {
        buffer_       = buffer;
        lfo_phase_[0] = 0.f;
        lfo_phase_[1] = 0.f;

        lfo_freq_[0] = 0.f;
        lfo_freq_[1] = 0.f;
    }

    void Clear()
    {
        std::fill(&buffer_[0], &buffer_[size], 0);
        write_ptr_ = 0;
    }

    struct Empty
    {
    };

    template <int32_t l, typename T = Empty>
    struct Reserve
    {
        typedef T Tail;
        enum
        {
            length = l
        };
    };

    template <typename Memory, int32_t index>
    struct DelayLine
    {
        enum
        {
            length = DelayLine<typename Memory::Tail, index - 1>::length,
            base   = DelayLine<Memory, index - 1>::base
                   + DelayLine<Memory, index - 1>::length + 1
        };
    };

    template <typename Memory>
    struct DelayLine<Memory, 0>
    {
        enum
        {
            length = Memory::length,
            base   = 0
        };
    };

    class Context
    {
        friend class FxEngine;

      public:
        Context() {}
        ~Context() {}

        inline void Load(float value) { accumulator_ = value; }

        inline void Read(float value, float scale)
        {
            accumulator_ += value * scale;
        }

        inline void Read(float value) { accumulator_ += value; }

        inline void Write(float& value) { value = accumulator_; }

        inline void Write(float& value, float scale)
        {
            value = accumulator_;
            accumulator_ *= scale;
        }

        template <typename D>
        inline void Write(D& d, int32_t offset, float scale)
        {
            STATIC_ASSERT(D::base + D::length <= size, delay_memory_full);
            T w = DataType<format>::Compress(accumulator_);
            if(offset == -1)
            {
                buffer_[(write_ptr_ + D::base + D::length - 1) & MASK] = w;
            }
            else
            {
                buffer_[(write_ptr_ + D::base + offset) & MASK] = w;
            }
            accumulator_ *= scale;
        }

        template <typename D>
        inline void Write(D& d, float scale)
        {
            Write(d, 0, scale);
        }

        template <typename D>
        inline void WriteAllPass(D& d, int32_t offset, float scale)
        {
            Write(d, offset, scale);
            accumulator_ += previous_read_;
        }

        template <typename D>
        inline void WriteAllPass(D& d, float scale)
        {
            WriteAllPass(d, 0, scale);
        }

        template <typename D>
        inline void Read(D& d, int32_t offset, float scale)
        {
            STATIC_ASSERT(D::base + D::length <= size, delay_memory_full);
            T r;
            if(offset == -1)
            {
                r = buffer_[(write_ptr_ + D::base + D::length - 1) & MASK];
            }
            else
            {
                r = buffer_[(write_ptr_ + D::base + offset) & MASK];
            }
            float r_f      = DataType<format>::Decompress(r);
            previous_read_ = r_f;
            accumulator_ += r_f * scale;
        }

        template <typename D>
        inline void Read(D& d, float scale)
        {
            Read(d, 0, scale);
        }

        inline void Lp(float& state, float coefficient)
        {
            state += coefficient * (accumulator_ - state);
            accumulator_ = state;
        }

        inline void Hp(float& state, float coefficient)
        {
            state += coefficient * (accumulator_ - state);
            accumulator_ -= state;
        }

        template <typename D>
        inline void Interpolate(D& d, float offset, float scale)
        {
            STATIC_ASSERT(D::base + D::length <= size, delay_memory_full);

            int32_t offset_integral = static_cast<int32_t>(offset);
            float   offset_fractional
                = offset - static_cast<float>(offset_integral);

            float a = DataType<format>::Decompress(
                buffer_[(write_ptr_ + offset_integral + D::base) & MASK]);
            float b = DataType<format>::Decompress(
                buffer_[(write_ptr_ + offset_integral + D::base + 1) & MASK]);
            float x        = a + (b - a) * offset_fractional;
            previous_read_ = x;
            accumulator_ += x * scale;
        }

        template <typename D>
        inline void Interpolate(D&       d,
                                float    offset,
                                LFOIndex index,
                                float    amplitude,
                                float    scale)
        {
            STATIC_ASSERT(D::base + D::length <= size, delay_memory_full);
            lfo_phase_[index] += lfo_freq_[index];
            lfo_phase_[index] = lfo_phase_[index] >= 1.f
                                    ? lfo_phase_[index] - 1.f
                                    : lfo_phase_[index];

            offset += amplitude * cosf(lfo_phase_[index] * TWOPI_F);

            int32_t offset_integral = static_cast<int32_t>(offset);
            float   offset_fractional
                = offset - static_cast<float>(offset_integral);

            float a = DataType<format>::Decompress(
                buffer_[(write_ptr_ + offset_integral + D::base) & MASK]);
            float b = DataType<format>::Decompress(
                buffer_[(write_ptr_ + offset_integral + D::base + 1) & MASK]);
            float x        = a + (b - a) * offset_fractional;
            previous_read_ = x;
            accumulator_ += x * scale;
        }

      private:
        float   accumulator_;
        float   previous_read_;
        float   lfo_value_[2];
        T*      buffer_;
        int32_t write_ptr_;

        float* lfo_phase_;
        float* lfo_freq_;
    };

    inline void SetLFOFrequency(LFOIndex index, float frequency)
    {
        lfo_freq_[index] = frequency; //* 32.f ?
    }

    inline void Start(Context* c)
    {
        --write_ptr_;
        if(write_ptr_ < 0)
        {
            write_ptr_ += size;
        }
        c->accumulator_   = 0.0f;
        c->previous_read_ = 0.0f;
        c->buffer_        = buffer_;
        c->write_ptr_     = write_ptr_;

        c->lfo_phase_ = lfo_phase_;
        c->lfo_freq_  = lfo_freq_;

        if((write_ptr_ & 31) == 0)
        {
            lfo_phase_[0] += lfo_freq_[0];
            lfo_phase_[0]
                = lfo_phase_[0] >= 1.f ? lfo_phase_[0] - 1.f : lfo_phase_[0];

            lfo_phase_[1] += lfo_freq_[1];
            lfo_phase_[1]
                = lfo_phase_[1] >= 1.f ? lfo_phase_[1] - 1.f : lfo_phase_[1];

            c->lfo_value_[0] = cosf(TWOPI_F * lfo_phase_[0]);
            c->lfo_value_[1] = cosf(TWOPI_F * lfo_phase_[1]);
        }
        else
        {
            c->lfo_value_[0] = cosf(TWOPI_F * lfo_phase_[0]);
            c->lfo_value_[1] = cosf(TWOPI_F * lfo_phase_[1]);
        }
    }

  private:
    enum
    {
        MASK = size - 1
    };

    int32_t write_ptr_;
    T*      buffer_;
    float   lfo_phase_[2];
    float   lfo_freq_[2];
};
} // namespace daisysp
#endif
#endif