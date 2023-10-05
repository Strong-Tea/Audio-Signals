#ifndef _WAVES_H_
#define _WAVES_H_

#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif 

typedef struct _wave_params{
    float  amplitude;                   // signal amplitude
    float  frequency;                   // signal frequency
    float  initial_phase;               // initial phase of the signal (offset).
    float  dutyCycle;                   // duty cycle (for pulsating signal).
    int    num_discrete_value;          // number of the audio signal point (sample number).
    int    sample_rate;                 // sampling rate of the audio signal.
} wave_params;


float sine_wave(wave_params* params);
float pulse_wave(wave_params* params);
float triangle_wave(wave_params* params);
float sawtooth_wave(wave_params* params);
float noise(wave_params* params);

typedef float (wave_fn)(wave_params*);

#endif /*_WAVES_H_*/