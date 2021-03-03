#pragma once
#ifndef __TEST_UTIL_H__
#define __TEST_UTIL_H__

#include <cstdint>
#include <cassert>
#include <cmath>
#include <float.h>
#include "daisysp.h"

#if defined(_WIN32)
#include "daisy_pc.h"

namespace daisy
{
/* alias everything as DaisyPC on Windows */
using DaisySeed = DaisyPC;
using DaisyPod  = DaisyPC;
} // namespace daisy

#else   // Daisy target
#include "daisy_seed.h"
#include "daisy_pod.h"
#include "util/scopedirqblocker.h"
#endif // WIN32/DAISY

/**   @brief Utility functions for implementing module unit tests
 *    @author Alexander Petrov-Savchenko (axp@soft-amp.com)
 *    @date December 2020
 */


namespace daisysp
{
/** Compile-time loop
 * The iteration is applied to the template class Fn, via functor ()
 * since direct usage of template function as template parameter is 
 * not possible in C++14
 * 
 * To be used as follows (example)
 * The "iteration body" would be executed with i stepping from 0 to 9:
 * Note: operator() returning bool is also supported via go_bool
 * 
 * template<size_t i>
 * class iterator
 * {
 *     public:
 *     void operator()()
 *     {
 *         // iteration body
 *     }
 * };
 * 
 * static_for<0, 10> loop;
 * loop.go<iterator>();
 */

/* Static loop - Recursion */
template <size_t from, size_t to>
struct static_for
{
    template <template <size_t> class Fn>
    static bool go_bool()
    {
        bool ret = Fn<from>()();
        ret &= static_for<from + 1, to>::template go_bool<Fn>();
        return ret;
    }

    template <template <size_t> class Fn>
    static void go()
    {
        Fn<from>()();
        static_for<from + 1, to>::template go<Fn>();
    }
};

/* Static loop - Termination */
template <size_t to>
struct static_for<to, to>
{
    template <template <size_t> class Fn>
    static bool go_bool()
    {
        return true;
    }

    template <template <size_t> class Fn>
    static void go()
    {
    }
};

/** Wrapper class for test utilities. 
 * DaisySeed and DaisyPod hw_type values are currently supported on ARM
 * and DaisyPC on Windows
 */
template <typename hw_type>
class DsyTestHelper
{
  public:
    DsyTestHelper() {}
    ~DsyTestHelper() {}


    void              Prepare();
    void              Finish(bool result);
    daisy::DaisySeed& GetSeed();


    template <typename... Args>
    static constexpr auto Print(Args&&... args)
        -> decltype(daisy::DaisySeed::Print(std::forward<Args>(args)...))
    {
        daisy::DaisySeed::Print(std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    static constexpr auto PrintLine(Args&&... args)
        -> decltype(daisy::DaisySeed::PrintLine(std::forward<Args>(args)...))
    {
        daisy::DaisySeed::PrintLine(std::forward<Args>(args)...);
    }

    static void GenerateSignal(float* buf, size_t length)
    {
        assert(nullptr != buf);

        if(nullptr != buf)
        {
            WhiteNoise nse;
            nse.Init();
            nse.SetAmp(1.0f);

            for(size_t i = 0; i < length; i++)
            {
                buf[i] = nse.Process();
            }
        }
    }
    static float
    CalcMSEdB(float* __restrict pSrcA, float* __restrict pSrcB, size_t length)
    {
        /* prone to error accumulation, so use double */
        double sum_error(0);
        for(size_t i = 0; i < length; i++)
        {
            if(isnanf(pSrcA[i]) || isnanf(pSrcB[i]))
            {
                return 200.0f;
            }
            const double error = pSrcA[i] - pSrcB[i];
            sum_error += error * error;
        }
        sum_error /= length;

        /* cap at -200dB */
        sum_error = DSY_MAX(sum_error, 1.0e-20);

        /* convert to dB (square root operation is factored in) */
        const float rms = 10.0f * fastlog10f((float)sum_error);

        return rms;
    }
    template <int N>
    static constexpr size_t FindMax(const size_t (&arr)[N])
    {
        size_t max_value(0);
        for(size_t i = 0; i < DSY_COUNTOF(arr); i++)
        {
            max_value = DSY_MAX(max_value, arr[i]);
        }
        return max_value;
    }

    static constexpr const char* ResultStr(bool result)
    {
        return str_res_[result];
    }

  protected:
    hw_type                      hw_;
    static constexpr const char* str_res_[] = {"FAIL", "PASS"};
}; // class DsyTestHelper

template <typename hw_type>
constexpr const char* DsyTestHelper<hw_type>::str_res_[];

#if defined(__arm__) // TODO: define a better suited symbol

/* Specialization for DaisySeed */
template <>
void DsyTestHelper<daisy::DaisySeed>::Prepare()
{
    hw_.Configure();
    hw_.Init();
    hw_.SetLed(true);
    hw_.StartLog(true);
}

template <>
void DsyTestHelper<daisy::DaisySeed>::Finish(bool result)
{
    PrintLine("Done: %s", ResultStr(result));
    hw_.SetLed(false);
}

template <>
daisy::DaisySeed& DsyTestHelper<daisy::DaisySeed>::GetSeed()
{
    return hw_;
}

/* Specialization for DaisyPod */


template <>
void DsyTestHelper<daisy::DaisyPod>::Prepare()
{
    daisy::Color led_init;
    led_init.Init(daisy::Color::WHITE);

    hw_.Init();
    hw_.led1.SetColor(led_init);
    hw_.led1.Update();
    hw_.seed.StartLog(true);
}

template <>
void DsyTestHelper<daisy::DaisyPod>::Finish(bool result)
{
    PrintLine("Done: %s", ResultStr(result));
    daisy::Color led_result;

    if(result)
    {
        led_result.Init(daisy::Color::GREEN);
    }
    else
    {
        led_result.Init(daisy::Color::RED);
    }

    hw_.led1.SetColor(led_result);
    hw_.led1.Update();
}

template <>
daisy::DaisySeed& DsyTestHelper<daisy::DaisyPod>::GetSeed()
{
    return hw_.seed;
}

#elif defined(_WIN32)

template <>
void DsyTestHelper<daisy::DaisyPC>::Prepare()
{
}

template <>
void DsyTestHelper<daisy::DaisyPC>::Finish(bool result)
{
    PrintLine("Done: %s", ResultStr(result));
}

template <>
daisy::DaisySeed& DsyTestHelper<daisy::DaisyPC>::GetSeed()
{
    /* this works since DaisySeed is aliased as DaisyPC */
    return hw_;
}

#endif

} // namespace daisysp

#endif //__TEST_UTIL_H__
