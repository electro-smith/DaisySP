#pragma once
#include <math.h>
#include <array>
#include <vector>

#include "dsp.h"

namespace daisysp
{

// NOTE: Using (size + 1) to fix the computational complexity with table wrapping
// i.e. data[i], data[i +1] where i == size
struct WaveBuffer
{
    static const uint16_t wt_size  = 2048;
    float                 top_freq = 0;
    float                 data[wt_size + 1]{0};
};

/**
 * @brief Abstraction around band-limited wavetables
 */
struct WaveTable
{
    WaveBuffer *buff = nullptr;

    float GetSample(float id)
    {
        // return interpolate_ ? Interpolate(id) : _get(static_cast<uint16_t>(id));
        return Interpolate(id);
    }

    /**
     * @brief Selects underlying waveform table matching the given normalized frequency.
     * 
     * @param norm_freq normalized frequency (frequency * 1 / sample_rate)
     */
    void SetTopFreq(float norm_freq)
    {
        if(buffers.size() == 1)
        {
            return;
        }

        size_t curr_buffer_ = 0;
        while(curr_buffer_ < buffers.size()
              && norm_freq >= buffers[curr_buffer_]->top_freq)
        {
            ++curr_buffer_;
        }

        curr_buffer = curr_buffer_;
        buff        = buffers[curr_buffer];
    }

    WaveBuffer *Allocate(WaveBuffer *buff_)
    {
        buff = buff_;
        buffers.push_back(buff);
        buff->top_freq = 0;
        return buffers.back();
    }

  private:
    float _get(size_t idx) { return buffers[curr_buffer]->data[idx]; }

    float Interpolate(float frame)
    {
        auto  intPart  = static_cast<uint16_t>(frame);
        float fracPart = frame - static_cast<float>(intPart);
        float samp0    = _get(intPart);
        float samp1    = _get(intPart + 1);
        return samp0 + (samp1 - samp0) * fracPart;
    }

    size_t curr_buffer = 0;

    std::vector<WaveBuffer *> buffers;
};

struct Tables
{
    static WaveTable Square;
    static WaveTable Sine;
    static WaveTable Tri;
    static WaveTable Saw;

    static void Generate()
    {
        if(generated)
        {
            return;
        }

        GenerateSine();
        GenerateTri();
        GenerateSaw();
        GenerateSquare();
        Tables::generated = true;
    }

  private:
    static WaveBuffer *Allocate(WaveTable *table)
    {
        auto buff_ = &buffer_pool[num_buffers];
        num_buffers++;

        return table->Allocate(buff_);
    }

    static void GenerateSine()
    {
        auto buff = Allocate(&Sine);
        auto step = static_cast<float>(2 * M_PI / size);
        for(size_t i = 0; i < size; i++)
        {
            buff->data[i] = sinf(i * step);
        }

        buff->data[size] = buff->data[0];
    }

    static void GenerateSquare()
    {
        Allocate(&Square);

        float freqWaveRe[size]; // real
        float freqWaveIm[size]; // imaginary

        auto half_table = (size / 2) - 1;
        for(auto idx = 0; idx < size; idx++)
        {
            freqWaveIm[idx] = idx > half_table ? -1 : 1;
            freqWaveRe[idx] = 0.0f;
        }
        fft(size, freqWaveRe, freqWaveIm);

        auto maxHarmonic = CalcHarmonics(freqWaveRe, freqWaveIm, size);
        fillTables(&Square, freqWaveRe, freqWaveIm, size, maxHarmonic);
    }

    static void GenerateSaw()
    {
        Allocate(&Saw);

        float freqWaveRe[size]; // real
        float freqWaveIm[size]; // imaginary

        // make a sawtooth
        for(auto idx = 0; idx < size; idx++)
        {
            freqWaveIm[idx] = 0.0;
        }

        freqWaveRe[0] = freqWaveRe[size >> 1] = 0.0;
        for(auto idx = 1; idx < (size >> 1); idx++)
        {
            freqWaveRe[idx]        = 1.f / idx;
            freqWaveRe[size - idx] = -freqWaveRe[idx];
        }

        auto maxHarmonic = CalcHarmonics(freqWaveRe, freqWaveIm, size);
        fillTables(&Saw, freqWaveRe, freqWaveIm, size, maxHarmonic);
    }

    static void GenerateTri()
    {
        Allocate(&Tri);

        float freqWaveRe[size]; // real
        float freqWaveIm[size]; // imaginary

        auto step = 2.f / size;
        for(auto idx = 0; idx < size; idx++)
        {
            auto t          = idx * step - 1;
            freqWaveIm[idx] = 2 * (fabsf(t) - 0.5f);
            freqWaveRe[idx] = 0.0f;
        }
        fft(size, freqWaveRe, freqWaveIm);

        auto maxHarmonic = CalcHarmonics(freqWaveRe, freqWaveIm, size);
        fillTables(&Tri, freqWaveRe, freqWaveIm, size, maxHarmonic);
    }

    static int
    CalcHarmonics(float *freqWaveRe, float *freqWaveIm, int numSamples)
    {
        // zero DC offset and Nyquist
        freqWaveRe[0] = freqWaveIm[0] = 0.0;
        freqWaveRe[numSamples >> 1] = freqWaveIm[numSamples >> 1] = 0.0;

        // determine maxHarmonic, the highest non-zero harmonic in the wave
        int         maxHarmonic = numSamples >> 1;
        const float minVal      = 0.000001f; // -120 dB
        while((fabs(freqWaveRe[maxHarmonic]) + fabs(freqWaveIm[maxHarmonic])
               < minVal)
              && maxHarmonic)
            --maxHarmonic;

        return maxHarmonic;
    }

    // The main function of interest here; call this with a pointer to an new, empty oscillator,
    // and the real and imaginary arrays and their length. The function fills the oscillator with
    // all wavetables necessary for full-bandwidth operation, based on one table per octave,
    // and returns the number of tables.
    static void fillTables(WaveTable *table,
                           float     *freqWaveRe,
                           float     *freqWaveIm,
                           int const  numSamples,
                           int        maxHarmonic)
    {
        // calculate topFreq for the initial wavetable
        // maximum non-aliasing playback rate is 1 / (2 * maxHarmonic), but we allow aliasing up to the
        // point where the aliased harmonic would meet the next octave table, which is an additional 1/3
        float topFreq = 2.0f / 3.0f / maxHarmonic;

        // for subsquent tables, float topFreq and remove upper half of harmonics
        float *ar = new float[numSamples];
        float *ai = new float[numSamples];

        float scale = 0.0;

        auto buff = table->buff;
        while(maxHarmonic)
        {
            // fill the table in with the needed harmonics
            for(auto idx = 0; idx < numSamples; idx++)
                ar[idx] = ai[idx] = 0.0;
            for(auto idx = 1; idx <= maxHarmonic; idx++)
            {
                ar[idx]              = freqWaveRe[idx];
                ai[idx]              = freqWaveIm[idx];
                ar[numSamples - idx] = freqWaveRe[numSamples - idx];
                ai[numSamples - idx] = freqWaveIm[numSamples - idx];
            }

            // make the wavetable
            fft(numSamples, ar, ai);
            scale = CalcScale(numSamples, ai, scale);

            if(AddWaveTable(buff, numSamples, ai, scale, topFreq))
                scale = 0.0;

            // prepare for next table
            topFreq *= 2;
            maxHarmonic >>= 1;
            if(maxHarmonic)
            {
                buff = Allocate(table);
            }
        }

        delete[] ai;
        delete[] ar;
    }

    //
    // if scale is 0, auto-scales
    // returns scaling factor (0.0 if failure), and wavetable in ai array
    //
    static float CalcScale(int len, float *ai, float scale)
    {
        if(scale == 0.0)
        {
            // calc normal
            float max = 0;
            for(int idx = 0; idx < len; idx++)
            {
                float temp = fabs(ai[idx]);
                if(max < temp)
                    max = temp;
            }
            scale = 1.0f / max * .999f;
        }

        return scale;
    }

    static int AddWaveTable(WaveBuffer  *buff,
                            int          len,
                            float const *waveTableIn,
                            float        scale,
                            float        topFreq)
    {
        buff->top_freq = topFreq;

        // fill in wave
        for(auto idx = 0; idx < len; idx++)
        {
            auto scaled     = waveTableIn[idx] * scale;
            buff->data[idx] = scaled;
        }
        // duplicate for interpolation wraparound
        buff->data[len] = buff->data[0];

        return 0;
    }

    /*
    in-place complex fft
    
    After Cooley, Lewis, and Welch; from Rabiner & Gold (1975)
    
    program adapted from FORTRAN 
    by K. Steiglitz  (ken@princeton.edu)
    Computer Science Dept. 
    Princeton University 08544          
    */
    static void fft(int numSamples, float *ar, float *ai)
    {
        int   i, j, k, L;           /* indexes */
        int   M, TEMP, LE, LE1, ip; /* M = log N */
        int   NV2, NM1;
        float t; /* temp */
        float Ur, Ui, Wr, Wi, Tr, Ti;
        float Ur_old;

        // if ((N > 1) && !(N & (N - 1)))   // make sure we have a power of 2

        NV2  = numSamples >> 1;
        NM1  = numSamples - 1;
        TEMP = numSamples; /* get M = log N */
        M    = 0;
        while(TEMP >>= 1)
            ++M;

        /* shuffle */
        j = 1;
        for(i = 1; i <= NM1; i++)
        {
            if(i < j)
            { /* swap a[i] and a[j] */
                t         = ar[j - 1];
                ar[j - 1] = ar[i - 1];
                ar[i - 1] = t;
                t         = ai[j - 1];
                ai[j - 1] = ai[i - 1];
                ai[i - 1] = t;
            }

            k = NV2; /* bit-reversed counter */
            while(k < j)
            {
                j -= k;
                k /= 2;
            }

            j += k;
        }

        LE = 1.;
        for(L = 1; L <= M; L++)
        {             // stage L
            LE1 = LE; // (LE1 = LE/2)
            LE *= 2;  // (LE = 2^L)
            Ur = 1.0;
            Ui = 0.;
            Wr = cos((float)(M_PI / LE1));
            Wi = -sin(
                (float)(M_PI / LE1)); // Cooley, Lewis, and Welch have "+" here
            for(j = 1; j <= LE1; j++)
            {
                for(i = j; i <= numSamples; i += LE)
                { // butterfly
                    ip         = i + LE1;
                    Tr         = ar[ip - 1] * Ur - ai[ip - 1] * Ui;
                    Ti         = ar[ip - 1] * Ui + ai[ip - 1] * Ur;
                    ar[ip - 1] = ar[i - 1] - Tr;
                    ai[ip - 1] = ai[i - 1] - Ti;
                    ar[i - 1]  = ar[i - 1] + Tr;
                    ai[i - 1]  = ai[i - 1] + Ti;
                }
                Ur_old = Ur;
                Ur     = Ur_old * Wr - Ui * Wi;
                Ui     = Ur_old * Wi + Ui * Wr;
            }
        }
    }

    static const uint16_t size{WaveBuffer::wt_size};

    static WaveBuffer buffer_pool[40];

    static uint8_t num_buffers;
    static bool    generated;
};

} // namespace daisysp
