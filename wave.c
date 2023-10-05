#include "wave.h"


/**
 * \brief The function takes a string and checks which function it refers to.
 * 
 * \return A Pointer to a function or NULL.
 */
wave_fn* parse_wave_fn(const char* arg) { 
    if (strcmp(arg, SINE_WAVE_STR) == 0) {
        return sine_wave;
    }
    if (strcmp(arg, PULSE_WAVE_STR) == 0) {
        return pulse_wave;
    }
    if (strcmp(arg, SAWTOOTH_WAVE_STR) == 0) {
        return sawtooth_wave;
    }
    if (strcmp(arg, TRIANGLE_WAVE_STR) == 0) {
        return triangle_wave;
    }
    if (strcmp(arg, NOISE_STR) == 0) {
        return noise;
    }

    fprintf(stderr, "cannot recognize wave: `%s`\n", arg);
    
    return NULL;
}


/**
 * \brief The function takes a string and checks which modulation it refers to.
 * 
 * \return An enum type that indicates modulation or MOD_NULL.
 */
wave_mod_type parse_mod_type(const char* arg) {
    if (strcmp(arg, AMP_MODULATION) == 0) {
        return MOD_AMPLITUDE;
    }
    if (strcmp(arg, FRQ_MODULATION) == 0) {
        return MOD_FREQUENCY;
    }

    fprintf(stderr, "cannot recognize modulation type: `%s`\n", arg);
    
    return MOD_NULL;
}


/**
 * \brief The function writes data to an audio file.
 * 
 * \context The `libsndfile` library is used to write to the file.
 * 
 * \return 0 in case of success, -1 in case of error.
 */
int  wr_wav_file(audio_signal *aud_signal) {
    SF_INFO sfinfo;
    sfinfo.samplerate = aud_signal->sample_rate;
    sfinfo.channels = 1;                                        // Mono channel
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;           // WAV format | Pulse Code Modulation 16 bit
    SNDFILE *outfile = sf_open(aud_signal->out_file, SFM_WRITE, &sfinfo);

    if (!outfile) {
        fprintf(stderr, "Error opening output file: %s\n", sf_strerror(outfile));
        return -1;
    }

    if (sf_write_float(outfile, aud_signal->samples, aud_signal->num_samples) < aud_signal->num_samples) {
        fprintf(stderr, "Error writing to the file: %s\n", sf_strerror(outfile));
        return -1;
    }

    sf_close(outfile);

    fprintf(stdout, "The signal is stored in %s. \n", aud_signal->out_file);
    return 0;
}


/**
 * \brief The function for generating a single signal.
 */
void gen_single_wav(audio_signal *aud_signal) {
    for (int i = 0; i < aud_signal->num_samples; i++) {
        aud_signal->carrier_params->num_discrete_value = i;
        aud_signal->samples[i] = aud_signal->carrier_fn(aud_signal->carrier_params);
    }
}


/**
 * \brief The Function for generating a combined signal.
 * You can use an array of different functions to create a single combined signal.
 */
void gen_merge_wav(audio_signal *aud_signal) {
    for (int i = 0; i < aud_signal->num_samples; i++) {
        aud_signal->carrier_params->num_discrete_value = i;
        for (int j = 0; j < aud_signal->cnt_mrg_fn; j++)
            aud_signal->samples[i] += aud_signal->merge_fn[j](aud_signal->carrier_params);
    }
}


/**
 * \brief The function for creating a modulated signal.
 * 
 * \context The function determines the type of signal modulation
 *  and uses the carrier and modulating signal to create a modulated signal.
 */
void gen_mod_wav(audio_signal *aud_signal) {
    if (aud_signal->mod_type == MOD_AMPLITUDE) {
        for (int i = 0; i < aud_signal->num_samples; i++) {
            aud_signal->carrier_params->num_discrete_value = i;
            aud_signal->modulating_params->num_discrete_value = i;
            aud_signal->samples[i] = (1.0 + aud_signal->modulating_fn(aud_signal->modulating_params)) * 
                aud_signal->carrier_fn(aud_signal->carrier_params);
        }
    }

    if (aud_signal->mod_type == MOD_FREQUENCY) {
        float summa = 0;
        float carrier_frequency = aud_signal->carrier_params->frequency;
        aud_signal->carrier_params->num_discrete_value = 1;
        for (int i = 0; i < aud_signal->num_samples; i++) {
            aud_signal->modulating_params->num_discrete_value = i;        
            summa += 1 + aud_signal->modulating_fn(aud_signal->modulating_params);
            aud_signal->carrier_params->frequency = carrier_frequency * summa;
            aud_signal->samples[i] = aud_signal->carrier_fn(aud_signal->carrier_params);
        }
    }
}


/**
 * \brief The function for freeing a memory of the `audio_signal` structure.
 * 
 * \context Use this function if you used dynamic memory allocation 
 * when parsing parameters passed from the console to the `audio_signal` structure.
 */
void destructor(audio_signal *aud_signal) {
    if (!aud_signal) {
        if (!aud_signal->carrier_params)
            free(aud_signal->carrier_params);
        if (!aud_signal->modulating_params)
            free(aud_signal->modulating_params);
        if (!aud_signal->samples)
            free(aud_signal->samples);
        if (!aud_signal->merge_fn) {
            for (int i = 0; i < aud_signal->cnt_mrg_fn; i++)
                free(aud_signal->merge_fn[i]);
            free(aud_signal->merge_fn);
        }
        free(aud_signal);
        aud_signal = NULL;
    }
}