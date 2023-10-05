#include <stdio.h>
#include "common.h"
#include "wave.h"

/**
 * \brief The help function that tells you in what order and quantity
 *  you need to pass parameters through the console.
 */
void help(const char *name) {
    fprintf(stdout, "usage: %s [out] [c] [ca] [cf] {pd} [n] [p] [d] \n", name);
    fprintf(stdout, "  out - out file path. \n"
                    "  c   - carrier wave (vals: 'sine', 'sawtooth', 'triangle', 'pulse', 'noise'). \n"
                    "  ca  - amplitude of carrier function. \n"
                    "  cf  - frequency of carrier function (in Hz). \n"
                    "  dc  - duty cycle. \n"
                    "  n   - sample rate (in Hz). \n"
                    "  p   - initial phase. \n"
                    "  d   - duration of .wav file to be generated. \n" 
    );
}


/**
 * \brief The function initializes the `audio_signal` structure.
 * 
 * \return Pointer to the `audio_signal` structure
 */
static audio_signal* parse_params(char** argv) {
    int index = 1;
    audio_signal *aud_signal = malloc(sizeof(audio_signal));
    aud_signal->carrier_params = malloc(sizeof(wave_params));

    aud_signal->out_file = argv[index++]; 
    aud_signal->carrier_fn = parse_wave_fn(argv[index++]);
    if (!aud_signal->carrier_fn)
        exit(EXIT_FAILURE);
    aud_signal->carrier_params->amplitude = strtod(argv[index++], NULL);
    aud_signal->carrier_params->frequency = strtod(argv[index++], NULL);
    aud_signal->carrier_params->dutyCycle = strtod(argv[index++], NULL);
    aud_signal->carrier_params->sample_rate = aud_signal->sample_rate = atoi(argv[index++]);
    aud_signal->carrier_params->initial_phase = strtod(argv[index++], NULL);
    aud_signal->signal_duration = strtod(argv[index++], NULL);
    aud_signal->num_samples = aud_signal->signal_duration * aud_signal->sample_rate;
    aud_signal->samples = malloc(aud_signal->num_samples * sizeof(float));

    return aud_signal;
}


int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 8 &&
        argc != 9) {
        help(argv[0]);
        exit(EXIT_FAILURE);
    }

    audio_signal *aud_signal = parse_params(argv);
       
    gen_single_wav(aud_signal);
    wr_wav_file(aud_signal);
    destructor(aud_signal);

    return 0;
}