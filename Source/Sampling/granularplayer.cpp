#include "granularplayer.h"

using namespace daisysp;

void GranularPlayer::Init(float* sample, int size, float sample_rate)
{
    /*initialize variables to private members*/
    sample_      = sample;
    size_        = size;
    sample_rate_ = sample_rate;
    /*initialize phasors. phs2_ is initialized with a phase offset of 0.5f to create an overlapping effect*/
    phs_.Init(sample_rate_, 0, 0);
    phsImp_.Init(sample_rate_, 0, 0);
    phs2_.Init(sample_rate_, 0, 0.5f);
    phsImp2_.Init(sample_rate_, 0, 0);
    /*calculate sample frequency*/
    sample_frequency_ = sample_rate_ / size_;
    /*initialize half cosine envelope*/
    for(int i = 0; i < 256; i++)
    {
        cosEnv_[i] = sinf((i / 256.0f) * M_PI);
    }
}

uint32_t GranularPlayer::WrapIdx(uint32_t idx, uint32_t sz)
{
    /*wraps idx to sz*/
    if(idx > sz)
    {
        idx = idx - sz;
        return idx;
    }

    return idx;
}

float GranularPlayer::CentsToRatio(float cents)
{
    /*converts cents to  ratio*/
    return powf(2.0f, cents / 1200.0f);
}


float GranularPlayer::MsToSamps(float ms, float samplerate)
{
    /*converts milliseconds to  number of samples*/
    return (ms * 0.001f) * samplerate;
}

float GranularPlayer::NegativeInvert(Phasor* phs, float frequency)
{
    /*inverts the phase of the phasor if the frequency is negative, mimicking pure data's phasor~ object*/
    return (frequency > 0) ? phs->Process() : ((phs->Process() * -1) + 1);
}

float GranularPlayer::Process(float speed,
                              float transposition,
                              float grain_size)
{
    grain_size_    = grain_size;
    speed_         = speed * sample_frequency_;
    transposition_ = (CentsToRatio(transposition) - speed)
                     * (grain_size >= 1 ? 1000 / grain_size_ : 1);
    phs_.SetFreq(fabs((speed_ / 2)));
    phs2_.SetFreq(fabs((speed_ / 2)));
    phsImp_.SetFreq(fabs(transposition_));
    phsImp2_.SetFreq(fabs(transposition_));
    idxSpeed_   = NegativeInvert(&phs_, speed_) * size_;
    idxSpeed2_  = NegativeInvert(&phs2_, speed_) * size_;
    idxTransp_  = (NegativeInvert(&phsImp_, transposition_)
                  * MsToSamps(grain_size_, sample_rate_));
    idxTransp2_ = (NegativeInvert(&phsImp2_, transposition_)
                   * MsToSamps(grain_size_, sample_rate_));
    idx_        = WrapIdx((uint32_t)(idxSpeed_ + idxTransp_), size_);
    idx2_       = WrapIdx((uint32_t)(idxSpeed2_ + idxTransp2_), size_);
    sig_        = sample_[idx_] * cosEnv_[(uint32_t)(phs_.Process() * 256)];
    sig2_       = sample_[idx2_] * cosEnv_[(uint32_t)(phs2_.Process() * 256)];
    return (sig_ + sig2_) / 2;
}