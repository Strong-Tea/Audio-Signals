#include "waves.h"


/**
 * \brief The function for creating a `sinusoidal signal`.
 * 
 * \return A value of the audio signal at a certain point (a certain time)
 */
float sine_wave(wave_params* params) { 
    return params->amplitude * sinf(2.0 * M_PI * params->frequency *
        params->num_discrete_value / params->sample_rate + params->initial_phase);
}


/**
 * \brief The function for creating a `pulsating signal`.
 * 
 * \return A value of the audio signal at a certain point (a certain time)
 */
float pulse_wave(wave_params* params) {
    return (fmod(2.0 * M_PI * params->frequency * params->num_discrete_value /
        params->sample_rate + params->initial_phase, 2 * M_PI) / 2.0 / M_PI) <=
        params->dutyCycle ? params->amplitude : -params->amplitude;
}


/**
 * \brief The function for creating a `triangular signal`.
 * 
 * \return A value of the audio signal at a certain point (a certain time)
 */
float triangle_wave(wave_params* params) {
    return 2.0 * params->amplitude / M_PI * asinf(sinf(
        2.0 * M_PI * params->frequency * params->num_discrete_value / params->sample_rate + params->initial_phase
    ));
}


/**
 * \brief The function for creating a `sawtooth signal`.
 * 
 * \return A value of the audio signal at a certain point (a certain time)
 */
float sawtooth_wave(wave_params* params) {
    return -2.0 * params->amplitude / M_PI * atanf(1.0 / tanf(
        M_PI * params->frequency * params->num_discrete_value / params->sample_rate + params->initial_phase
    ));
}


/**
 * \brief The function for creating a `white noise signal`.
 * 
 * \return A value of the audio signal at a certain point (a certain time)
 */
float noise(wave_params* params) {
    return ((float)rand() / RAND_MAX) * 2.0 * params->amplitude - params->amplitude;
}