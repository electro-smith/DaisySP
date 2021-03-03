#include "daisysp.h"
#include "test_util.h"

#if defined(_WIN32)

#else
#include "util/scopedirqblocker.h"
#endif

/**   @brief FIR Filter unit tests / benchmarks
 *    @author Alexander Petrov-Savchenko (axp@soft-amp.com)
 *    @date February 2021
 */

using namespace daisysp;
using namespace daisy;


/** Test platform choice, DaisySeed, DaisyPod and DaisyPC are currently supported 
 ** If compiled for a PC target, all platforms would automagically turn into 
 ** DaisyPC */
using TestPlatform = DsyTestHelper<DaisyPod>;
static TestPlatform hw;


/* Test cases */
static constexpr size_t filter_list[]
    = {32, 40, 51, 64, 81, 102, 128, 161, 203, 256};
static constexpr size_t block_list[]
    = {1, 2, 4, 8, 16, 21, 32, 48, 64, 100, 128, 300};

/* Success criterion */
static constexpr float ERROR_THRESH_DB = -199.0f;

/* Compile-time bounds */
static constexpr size_t MAX_IR_LENGTH = TestPlatform::FindMax(filter_list);
static constexpr size_t MAX_BLOCK_SZ  = TestPlatform::FindMax(block_list);
static constexpr size_t SIGNAL_LENGTH = 65536;

/* Memory buffers */
static float DSY_SDRAM_BSS data_in[SIGNAL_LENGTH];
static float DSY_SDRAM_BSS data_out[SIGNAL_LENGTH];
static float DSY_SDRAM_BSS data_ref[SIGNAL_LENGTH];
static float DSY_SDRAM_BSS data_ir[MAX_IR_LENGTH];
static float DSY_SDRAM_BSS fir_state[MAX_IR_LENGTH + MAX_BLOCK_SZ - 1];

/** Helper function to apply a given filter 
  * Works with both fixed and dynamic filters 
  */
template <class dut_type>
static uint32_t apply(dut_type& DUT,
                      const float* __restrict pFilter,
                      float* __restrict pSrc,
                      float* __restrict pDst,
                      size_t filter_length,
                      size_t signal_length,
                      size_t block_size)
{
    assert(nullptr != pFilter);
    assert(nullptr != pSrc);
    assert(nullptr != pDst);
    assert(filter_length > 0);

    /* configure impulse response */
    const bool init_res = DUT.SetIR(pFilter, filter_length, false);
    if(false == init_res)
    {
        hw.PrintLine("FIR init result: FAIL");
        return 0;
    }

    uint32_t dt;
    if(block_size == 1)
    {
        /* disable interrupts for the duration of measurements */
        ScopedIrqBlocker block;
        const uint32_t   t0 = hw.GetSeed().system.GetTick();

        for(size_t i = 0; i < signal_length; i++)
        {
            pDst[i] = DUT.Process(pSrc[i]); /*< process sample by sample */
        }

        dt = hw.GetSeed().system.GetTick() - t0;
    }
    else
    {
        /* disable interrupts for the duration of measurements */
        ScopedIrqBlocker block;
        const uint32_t   t0 = hw.GetSeed().system.GetTick();

        while(signal_length >= block_size)
        {
            /* process whole blocks */
            DUT.ProcessBlock(pSrc, pDst, block_size);
            signal_length -= block_size;
            pSrc += block_size;
            pDst += block_size;
        }
        if(signal_length > 0)
        {
            /* process whatever is left */
            DUT.ProcessBlock(pSrc, pDst, signal_length);
        }

        dt = hw.GetSeed().system.GetTick() - t0;
    }

    /* return time delta in ticks */
    return dt;
}

/* Dynamically configurable shared objects */
static FIR<FIRFILTER_USER_MEMORY>                  DUT0;
static FIRFilterImplGeneric<FIRFILTER_USER_MEMORY> REF0;
/* Only compare/benchmark DUT against REF if they are different classes */
static constexpr bool compare_dut_ref = !std::is_same< FIR<FIRFILTER_USER_MEMORY>, FIRFilterImplGeneric<FIRFILTER_USER_MEMORY> >();


template <size_t filter_length>
static bool
verify_firfilter_single(float* filter, size_t signal_length, size_t block_size)
{
    /* Compile-time configured object */
    static FIRFilterImplGeneric<filter_length, MAX_BLOCK_SZ> REF;

    assert(block_size <= MAX_BLOCK_SZ);
    assert(signal_length <= DSY_COUNTOF(data_out));
    assert(signal_length <= DSY_COUNTOF(data_ref));
    assert(signal_length <= DSY_COUNTOF(data_in));

    /* regenerate input signal every time for some random variation */
    hw.GenerateSignal(data_in, signal_length);
    memset(data_out, 0, sizeof(data_out));
    memset(data_ref, 0, sizeof(data_ref));

    const float tick_freq = 2.0e-6f * hw.GetSeed().system.GetPClk1Freq();
    const float per_scale = 1.0f / (tick_freq * signal_length);

    /* Run extended verification and benchmarks if more than one implementation is available */
    if (compare_dut_ref)
    {
        /* Another compile-time configured object for comparison */
        static FIR<filter_length, MAX_BLOCK_SZ> DUT;

        /* apply FIR filters and calcluate error magnitudes */
        const uint32_t ref_dt = apply(REF,
            filter,
            data_in,
            data_ref,
            filter_length,
            signal_length,
            block_size);

        const uint32_t dut_dt = apply(DUT,
            filter,
            data_in,
            data_out,
            filter_length,
            signal_length,
            block_size);
        const float    rms_rd = hw.CalcMSEdB(data_ref, data_out, signal_length);

        const uint32_t ref0_dt = apply(REF0,
            filter,
            data_in,
            data_out,
            filter_length,
            signal_length,
            block_size);
        const float    rms_r0 = hw.CalcMSEdB(data_ref, data_out, signal_length);

        const uint32_t dut0_dt = apply(DUT0,
            filter,
            data_in,
            data_out,
            filter_length,
            signal_length,
            block_size);
        const float    rms_d0 = hw.CalcMSEdB(data_ref, data_out, signal_length);

        /* verification criteria */
        const bool pass = rms_rd < ERROR_THRESH_DB && rms_r0 < ERROR_THRESH_DB
            && rms_d0 < ERROR_THRESH_DB;

        /* produce human-readable forms */
        const float dut_time = per_scale * dut_dt;
        const float ref_time = per_scale * ref_dt;
        const float dut0_time = per_scale * dut0_dt;
        const float ref0_time = per_scale * ref0_dt;

        /* print the results */
        hw.PrintLine("%5u |%6u |" FLT_FMT3 "|" FLT_FMT3 "|" FLT_FMT3 "|" FLT_FMT3
            " | " FLT_FMT3 " | " FLT_FMT3 " | " FLT_FMT3 "| %s",
            filter_length,
            block_size,
            FLT_VAR3(rms_rd),
            FLT_VAR3(rms_r0),
            FLT_VAR3(rms_d0),
            FLT_VAR3(ref_time),
            FLT_VAR3(dut_time),
            FLT_VAR3(ref0_time),
            FLT_VAR3(dut0_time),
            hw.ResultStr(pass));

        return pass;
    }
    else /* compare_dut_ref (FIR and FIRFilterImplGeneric are the same) */
    {
        /* apply FIR filters and calcluate error magnitudes */
        const uint32_t ref_dt = apply(REF,
            filter,
            data_in,
            data_ref,
            filter_length,
            signal_length,
            block_size);

        const uint32_t ref0_dt = apply(REF0,
            filter,
            data_in,
            data_out,
            filter_length,
            signal_length,
            block_size);
        const float    rms_r0 = hw.CalcMSEdB(data_ref, data_out, signal_length);

        /* verification criteria */
        const bool pass = rms_r0 < ERROR_THRESH_DB;

        /* produce human-readable forms */
        const float ref_time  = per_scale * ref_dt;
        const float ref0_time = per_scale * ref0_dt;

        /* print the results */
        hw.PrintLine("%5u |%6u |        |" FLT_FMT3 "|        |" FLT_FMT3
            " |        | " FLT_FMT3 " |       | %s",
            filter_length,
            block_size,
            
            FLT_VAR3(rms_r0),
            
            FLT_VAR3(ref_time),
            FLT_VAR3(ref0_time),
            hw.ResultStr(pass));

        return pass;
    }
}


template <size_t i>
struct verifier
{
    bool operator()()
    {
        bool result = true;
        for(size_t j = 0; j < DSY_COUNTOF(block_list); j++)
        {
            result &= verify_firfilter_single<filter_list[i]>(
                data_ir, DSY_COUNTOF(data_in), block_list[j]);
        }
        return result;
    }
};


int main(void)
{
    /* Initialize hardware */
    hw.Prepare();

    /* Randomize impulse response */
    hw.GenerateSignal(data_ir, DSY_COUNTOF(data_ir));

    /* Configure user-memory variants (OK to share the state) */
    DUT0.SetStateBuffer(fir_state, DSY_COUNTOF(fir_state));
    REF0.SetStateBuffer(fir_state, DSY_COUNTOF(fir_state));
    
    /* Print header */
    hw.PrintLine(
        "Filter| Proc  | RMS Error vs FixRef [dB] |    Processing Time "
        "[us/smp]     |");
    hw.PrintLine(
        "  IR  | Block |  Fixed | User-allocated  |  Fixed memory  | "
        "User-allocated | Check");
    hw.PrintLine(
        " Size | Size  |   ARM  |   Ref  |  ARM   |  Ref  |   ARM  |   Ref  |  "
        "ARM  |");

    /* Iterate through all filter configurations */
    static_for<0, DSY_COUNTOF(filter_list)> loop;

    const bool result = loop.go_bool<verifier>();

    /* Display the result */
    hw.Finish(result);
    return result ? 0 : -1;
}
