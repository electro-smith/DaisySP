#include "drip.h"
#include <math.h>

using namespace daisysp;

#define WUTR_SOUND_DECAY 0.95
#define WUTR_SYSTEM_DECAY 0.996
#define WUTR_GAIN 1.0
#define WUTR_NUM_SOURCES 10.0
#define WUTR_CENTER_FREQ0 450.0
#define WUTR_CENTER_FREQ1 600.0
#define WUTR_CENTER_FREQ2 750.0
#define WUTR_RESON 0.9985
#define WUTR_FREQ_SWEEP 1.0001
#define MAX_SHAKE 2000

int Drip::my_random(int max)
{
    return (rand() % (max + 1));
}

float Drip::noise_tick()
{
    float temp;
    temp = 1.0 * rand() - 1073741823.5;
    return temp * (1.0 / 1073741823.0);
}

void Drip::Init(float sample_rate, float dettack)
{
    sample_rate_ = sample_rate;
    float temp;
    dettack_   = dettack;
    num_tubes_ = 10;
    damp_      = 0.2;
    shake_max_ = 0;
    freq_      = 450.0;
    freq1_     = 600.0;
    freq2_     = 720.0;
    amp_       = 0.3;

    snd_level_   = 0.0;
    float tpidsr = 2.0 * M_PI / sample_rate_;

    kloop_     = (sample_rate_ * dettack_);
    outputs00_ = 0.0;
    outputs01_ = 0.0;
    outputs10_ = 0.0;
    outputs11_ = 0.0;
    outputs20_ = 0.0;
    outputs21_ = 0.0;

    total_energy_ = 0.0;

    center_freqs0_ = res_freq0_ = WUTR_CENTER_FREQ0;
    center_freqs1_ = res_freq1_ = WUTR_CENTER_FREQ1;
    center_freqs2_ = res_freq2_ = WUTR_CENTER_FREQ2;
    num_objects_save_ = num_objects_ = WUTR_NUM_SOURCES;
    sound_decay_                     = WUTR_SOUND_DECAY;
    system_decay_                    = WUTR_SYSTEM_DECAY;
    temp    = logf(WUTR_NUM_SOURCES) * WUTR_GAIN / WUTR_NUM_SOURCES;
    gains0_ = gains1_ = gains2_ = temp;
    coeffs01_                   = WUTR_RESON * WUTR_RESON;
    coeffs00_ = -WUTR_RESON * 2.0 * cos(WUTR_CENTER_FREQ0 * tpidsr);
    coeffs11_ = WUTR_RESON * WUTR_RESON;
    coeffs10_ = -WUTR_RESON * 2.0 * cos(WUTR_CENTER_FREQ1 * tpidsr);
    coeffs21_ = WUTR_RESON * WUTR_RESON;
    coeffs20_ = -WUTR_RESON * 2.0 * cos(WUTR_CENTER_FREQ2 * tpidsr);

    shake_energy_ = amp_ * 1.0 * MAX_SHAKE * 0.1;
    shake_damp_   = 0.0;
    if(shake_energy_ > MAX_SHAKE)
        shake_energy_ = MAX_SHAKE;
    shake_max_save_ = 0.0;
    num_objects_    = 10;
    finalZ0_ = finalZ1_ = finalZ2_ = 0.0;
}

float Drip::Process(bool trig)
{
    float data;
    float lastOutput;

    float tpidsr = 2.0 * M_PI / sample_rate_;

    if(trig)
    {
        Init(sample_rate_, dettack_);
    }
    if(num_tubes_ != 0.0 && num_tubes_ != num_objects_)
    {
        num_objects_ = num_tubes_;
        if(num_objects_ < 1.0)
        {
            num_objects_ = 1.0;
        }
    }
    if(freq_ != 0.0 && freq_ != res_freq0_)
    {
        res_freq0_ = freq_;
        coeffs00_  = -WUTR_RESON * 2.0 * cos(res_freq0_ * tpidsr);
    }
    if(damp_ != 0.0 && damp_ != shake_damp_)
    {
        shake_damp_   = damp_;
        system_decay_ = WUTR_SYSTEM_DECAY + (shake_damp_ * 0.002);
    }
    if(shake_max_ != 0.0 && shake_max_ != shake_max_save_)
    {
        shake_max_save_ = shake_max_;
        shake_energy_ += shake_max_save_ * MAX_SHAKE * 0.1;
        if(shake_energy_ > MAX_SHAKE)
            shake_energy_ = MAX_SHAKE;
    }
    if(freq1_ != 0.0 && freq1_ != res_freq1_)
    {
        res_freq1_ = freq1_;
        coeffs10_  = -WUTR_RESON * 2.0 * cos(res_freq1_ * tpidsr);
    }
    if(freq2_ != 0.0 && freq2_ != res_freq2_)
    {
        res_freq2_ = freq2_;
        coeffs20_  = -WUTR_RESON * 2.0 * cos(res_freq2_ * tpidsr);
    }
    if((--kloop_) == 0)
    {
        shake_energy_ = 0.0;
    }

    float shakeEnergy = shake_energy_;
    float systemDecay = system_decay_;
    float sndLevel    = snd_level_;
    float num_objects = num_objects_;
    float soundDecay  = sound_decay_;
    float inputs0, inputs1, inputs2;

    shakeEnergy *= systemDecay; /* Exponential system decay */

    sndLevel = shakeEnergy;
    if(my_random(32767) < num_objects)
    {
        int j;
        j = my_random(3);
        if(j == 0)
        {
            center_freqs0_ = res_freq1_ * (0.75 + (0.25 * noise_tick()));
            gains0_        = fabs(noise_tick());
        }
        else if(j == 1)
        {
            center_freqs1_ = res_freq1_ * (1.0 + (0.25 * noise_tick()));
            gains1_        = fabs(noise_tick());
        }
        else
        {
            center_freqs2_ = res_freq1_ * (1.25 + (0.25 * noise_tick()));
            gains2_        = fabs(noise_tick());
        }
    }

    gains0_ *= WUTR_RESON;
    if(gains0_ > 0.001)
    {
        center_freqs0_ *= WUTR_FREQ_SWEEP;
        coeffs00_ = -WUTR_RESON * 2.0 * cos(center_freqs0_ * tpidsr);
    }
    gains1_ *= WUTR_RESON;
    if(gains1_ > 0.00)
    {
        center_freqs1_ *= WUTR_FREQ_SWEEP;
        coeffs10_ = -WUTR_RESON * 2.0 * cos(center_freqs1_ * tpidsr);
    }
    gains2_ *= WUTR_RESON;
    if(gains2_ > 0.001)
    {
        center_freqs2_ *= WUTR_FREQ_SWEEP;
        coeffs20_ = -WUTR_RESON * 2.0 * cos(center_freqs2_ * tpidsr);
    }

    sndLevel *= soundDecay;
    inputs0 = sndLevel;
    inputs0 *= noise_tick();
    inputs1 = inputs0 * gains1_;
    inputs2 = inputs0 * gains2_;
    inputs0 *= gains0_;
    inputs0 -= outputs00_ * coeffs00_;
    inputs0 -= outputs01_ * coeffs01_;
    outputs01_ = outputs00_;
    outputs00_ = inputs0;
    data       = gains0_ * outputs00_;
    inputs1 -= outputs10_ * coeffs10_;
    inputs1 -= outputs11_ * coeffs11_;
    outputs11_ = outputs10_;
    outputs10_ = inputs1_;
    data += gains1_ * outputs10_;
    inputs2 -= outputs20_ * coeffs20_;
    inputs2 -= outputs21_ * coeffs21_;
    outputs21_ = outputs20_;
    outputs20_ = inputs2_;
    data += gains2_ * outputs20_;

    finalZ2_ = finalZ1_;
    finalZ1_ = finalZ0_;
    finalZ0_ = data * 4.0;

    lastOutput = finalZ2_ - finalZ0_;
    lastOutput *= 0.005;
    shake_energy_ = shakeEnergy;
    snd_level_    = sndLevel;
    return lastOutput;
}
