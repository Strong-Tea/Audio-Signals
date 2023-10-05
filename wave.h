#ifndef _WAVE_H_
#define _WAVE_H_

#include <sndfile.h>
#include <string.h>
#include "waves.h"
#include "common.h"

typedef enum wave_modulation_type {
    MOD_NULL,                               // no signal modulation.
    MOD_AMPLITUDE,                          // modulation of the signal by amplitude.
    MOD_FREQUENCY,                          // modulation of the signal by frequency.
    MOD_PHASE                               // modulation of the signal by phase.
} wave_mod_type;


typedef struct {
    int           num_samples;              // number of points (samples) in the audio signal.
    int           sample_rate;              // sampling rate of the audio signal.
    float         signal_duration;          // duration of the audio signal in seconds.
    float         *samples;                 // an array of audio signal points (samples).
    wave_params   *carrier_params;          // parameters of the carrier signal.
    wave_params   *modulating_params;       // parameters of the modulating signal.
    wave_fn       *carrier_fn;              // carrier signal function.
    wave_fn       *modulating_fn;           // modulating signal function.
    wave_fn       **merge_fn;               // array of functions for combining signals.
    int           cnt_mrg_fn;               // number of functions for combining signals.
    wave_mod_type mod_type;                 // type of signal modulation.
    char          *out_file;                // output file.
} audio_signal;


wave_fn* parse_wave_fn(const char* arg);
wave_mod_type parse_mod_type(const char* arg);
int  wr_wav_file(audio_signal *aud_signal);
void gen_single_wav(audio_signal *aud_signal);
void gen_mod_wav(audio_signal *aud_signal);
void gen_merge_wav(audio_signal *aud_signal);
void destructor(audio_signal *aud_signal);

#endif /*_WAVE_H_*/