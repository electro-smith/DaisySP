/*
Copyright (c) 2020 Electrosmith, Corp, Alexander Petrov-Savchenko

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_FIRFILTER_H
#define DSY_FIRFILTER_H

#include <cstdint>
#include <cstring> // for memset
#include <cassert>
#include <utility>

#ifdef USE_ARM_DSP
#include <arm_math.h> // required for platform-optimized version
#endif

/**   @brief FIR Filter implementation, generic and ARM CMSIS DSP based
 *    @author Alexander Petrov-Savchenko (axp@soft-amp.com)
 *    @date February 2021
 */

namespace daisysp
{
/* use this as a template parameter to indicate user-provided memory storage */
#define FIRFILTER_USER_MEMORY 0, 0

/** Helper class that defines the memory model - internal or user-provided 
 * \param max_size - maximal filter length
 * \param max_block - maximal length of the block processing
 * if both parameters are 0, does NOT allocate any memory and instead
 * requires user-provided memory blocks to be passed as parameters.
 *
 * Not intended to be used directly, so constructor is not exposed
 */
template <size_t max_size, size_t max_block>
struct FIRMemory
{
    /* Public part of the API to be passed through to the FIR users */
  public:
    /* Reset the internal filter state (but not the coefficients) */
    void Reset() { memset(state_, 0, state_size_ * sizeof(state_[0])); }


  protected:
    FIRMemory() : state_{0}, coefs_{0}, size_(0) {}

    /* Expression for the maximum block size */
    static constexpr size_t MaxBlock() { return max_block; }

    /** Configure the filter coefficients
     * \param coefs - pointer to coefficients (tail-first order)
     * \param size - number of coefficients pointed by coefs (filter length)
     * \param reverse - flag to perform reversing of the filter
     * \return true if all conditions are met and the filter is configured
     */
    bool SetCoefs(const float coefs[], size_t size, bool reverse)
    {
        /* truncate silently */
        size_ = DSY_MIN(size, max_size);

        if(reverse)
        {
            /* reverse the IR */
            for(size_t i = 0; i < size_; i++)
            {
                /* start from size, not size_! */
                coefs_[i] = coefs[size - 1u - i];
            }
        }
        else
        {
            /* just copy as is */
            memcpy(coefs_, coefs, size_ * sizeof(coefs[0]));
        }

        return true;
    }

    static constexpr size_t state_size_ = max_size + max_block - 1u;
    float                   state_[state_size_]; /*< Internal state buffer */
    float                   coefs_[max_size];    /*< Filter coefficients */
    size_t                  size_; /*< Active filter length (<= max_size) */
};

/* Specialization for user-provided memory */
template <>
struct FIRMemory<FIRFILTER_USER_MEMORY>
{
    /* Public part of the API to be passed through to the FIRFilter user */
  public:
    /** Set user-provided state buffer
     * \param state - pointer to the allocated memory block
     * \param length - length of the provided memory block (in elements)
     * The length should be determined as follows 
     * length >= max_filter_size + max_processing_block - 1
     */
    void SetStateBuffer(float state[], size_t length)
    {
        state_      = state;
        state_size_ = length;
    }
    /* Reset the internal filter state (but not the coefficients) */
    void Reset()
    {
        assert(nullptr != state_);
        assert(0 != state_size_);
        if(nullptr != state_)
        {
            memset(state_, 0, state_size_ * sizeof(state_[0]));
        }
    }

  protected:
    FIRMemory() : state_(nullptr), coefs_(nullptr), size_(0), state_size_(0) {}

    /* Expression for the maximum processing block size currently supported */
    size_t MaxBlock() const
    {
        return state_size_ + 1u > size_ ? state_size_ + 1u - size_ : 0;
    }

    /** Configure the filter coefficients
     * \param coefs - pointer to coefficients (tail-first order)
     * \param size - number of coefficients pointed by coefs (filter length)
     * \param reverse - flag to perform reversing of the filter
     * \return true if all conditions are met and the filter is configured
     */
    bool SetCoefs(const float coefs[], size_t size, bool reverse)
    {
        /* reversing of external IR is not supported*/
        assert(false == reverse);
        assert(nullptr != coefs || 0 == size);

        if(false == reverse && (nullptr != coefs || 0 == size))
        {
            coefs_ = coefs;
            size_  = size;
            return true;
        }

        return false;
    }

    /* Internal member variables */

    float*       state_;      /*< Internal state buffer */
    const float* coefs_;      /*< Filter coefficients */
    size_t       size_;       /*< number of filter coefficients */
    size_t       state_size_; /*< length of the state buffer */
};


/** Generic FIR implementation is always available
 * \param max_size - maximal filter length
 * \param max_block - maximal block size for ProcessBlock()
 * if both parameters are 0 (via FIRFILTER_USER_MEMORY macro)
 * Assumes the user will provide own memory buffers
 * via SetIR() and SetStateBuffer() functions
 * Otherwise statically allocates the necessary buffers itself
 */
template <size_t max_size, size_t max_block>
class FIRFilterImplGeneric : public FIRMemory<max_size, max_block>
{
  private:
    using FIRMem = FIRMemory<max_size, max_block>; // just a shorthand

  public:
    /* Default constructor */
    FIRFilterImplGeneric() {}

    /* Reset filter state (but not the coefficients) */
    using FIRMem::Reset;

    /* FIR Latency is always 0, but API is unified with FFT and fast convolution */
    static constexpr size_t GetLatency() { return 0; }

    /* Process one sample at a time */
    float Process(float in)
    {
        assert(size_ > 0u);
        /* Feed data into the buffer */
        state_[size_ - 1u] = in;

        /* Convolution loop */
        float acc(0);
        for(size_t i = 0; i < size_ - 1; i++)
        {
            acc += state_[i] * coefs_[i];
            /** Shift the state simulatenously 
             * (note: better solutions are available) 
             */
            state_[i] = state_[1 + i];
        }
        acc += in * coefs_[size_ - 1u];

        return acc;
    }

    /* Process a block of data */
    void ProcessBlock(const float* pSrc, float* pDst, size_t block)
    {
        /* be sure to run debug version from time to time */
        assert(block <= FIRMem::MaxBlock());
        assert(size_ > 0u);
        assert(nullptr != pSrc);
        assert(nullptr != pDst);

        /* Process the block of data */
        for(size_t j = 0; j < block; j++)
        {
            /* Feed data into the buffer */
            state_[size_ - 1u + j] = pSrc[j];

            /* Convolution loop */
            float acc = 0.0f;
            for(size_t i = 0; i < size_; i++)
            {
                acc += state_[j + i] * coefs_[i];
            }

            /* Write output */
            pDst[j] = acc;
        }

        /* Copy data tail for the next block */
        for(size_t i = 0; i < size_ - 1u; i++)
        {
            state_[i] = state_[block + i];
        }
    }

    /** Set filter coefficients (aka Impulse Response)
     * Coefficients need to be in reversed order (tail-first)
     * If internal storage is used, makes a local copy
     * and allows reversing the impulse response
     */
    bool SetIR(const float* ir, size_t len, bool reverse)
    {
        /* Function order is important */
        const bool result = FIRMem::SetCoefs(ir, len, reverse);
        Reset();
        return result;
    }

    /* Create an alias to comply with DaisySP API conventions */
    template <typename... Args>
    inline auto Init(Args&&... args)
        -> decltype(SetIR(std::forward<Args>(args)...))
    {
        return SetIR(std::forward<Args>(args)...);
    }


  protected:
    using FIRMem::coefs_; /*< FIR coefficients buffer or pointer */
    using FIRMem::size_;  /*< FIR length */
    using FIRMem::state_; /*< FIR state buffer or pointer */
};


#if(defined(USE_ARM_DSP) && defined(__arm__))

/** ARM-specific FIR implementation, expose only on __arm__ platforms
 * \param max_size - maximal filter length
 * \param max_block - maximal block size for ProcessBlock()
 * if both parameters are 0 (via FIRFILTER_USER_MEMORY macro)
 * Assumes the user will provide own memory buffers
 * Otherwise statically allocates the necessary buffers
 */
template <size_t max_size, size_t max_block>
class FIRFilterImplARM : public FIRMemory<max_size, max_block>
{
  private:
    using FIRMem = FIRMemory<max_size, max_block>; // just a shorthand

  public:
    /* Default constructor */
    FIRFilterImplARM() : fir_{0} {}

    /* Reset filter state (but not the coefficients) */
    using FIRMem::Reset;

    /* FIR Latency is always 0, but API is unified with FFT and FastConv */
    static constexpr size_t GetLatency() { return 0; }

    /* Process one sample at a time */
    float Process(float in)
    {
        float out(0);
        arm_fir_f32(&fir_, &in, &out, 1);
        return out;
    }

    /* Process a block of data */
    void ProcessBlock(float* pSrc, float* pDst, size_t block)
    {
        assert(block <= FIRMem::MaxBlock());
        arm_fir_f32(&fir_, pSrc, pDst, block);
    }

    /** Set filter coefficients (aka Impulse Response)
     * Coefficients need to be in reversed order (tail-first)
     * If internal storage is used, makes a local copy
     * and allows reversing the impulse response
     */
    bool SetIR(const float* ir, size_t len, bool reverse)
    {
        /* Function order is important */
        const bool result = FIRMem::SetCoefs(ir, len, reverse);
        arm_fir_init_f32(&fir_, len, (float*)coefs_, state_, max_block);
        return result;
    }

    /* Create an alias to comply with DaisySP API conventions */
    template <typename... Args>
    inline auto Init(Args&&... args)
        -> decltype(SetIR(std::forward<Args>(args)...))
    {
        return SetIR(std::forward<Args>(args)...);
    }

  protected:
    arm_fir_instance_f32 fir_; /*< ARM CMSIS DSP library FIR filter instance */
    using FIRMem::coefs_;      /*< FIR coefficients buffer or pointer */
    using FIRMem::size_;       /*< FIR length*/
    using FIRMem::state_;      /*< FIR state buffer or pointer */
};


/* default to ARM implementation */
template <size_t max_size, size_t max_block>
using FIR = FIRFilterImplARM<max_size, max_block>;


#else // USE_ARM_DSP

/* default to generic implementation */
template <size_t max_size, size_t max_block>
using FIR = FIRFilterImplGeneric<max_size, max_block>;

#endif // USE_ARM_DSP


} // namespace daisysp

#endif // DSY_FIRFILTER_H
