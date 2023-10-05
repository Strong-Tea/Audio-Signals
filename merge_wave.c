#include <stdio.h>
#include "common.h"
#include "wave.h"


/**
 * \brief The help function that tells you in what order and quantity
 *  you need to pass parameters through the console.
 */
static void help(const char* name) {
    fprintf(stderr, "usage: %s [out] [a] [f] [dc] [n] [p] [d] [w1, w2 ... wn] \n", name);
    fprintf(stderr, "  out - out file path. \n"
                    "  a   - amplitude of wave function. \n"
                    "  f   - frequency of wave function (in Hz). \n"
                    "  dc  - duty cycle. \n"
                    "  n   - sample rate (in Hz). \n"
                    "  p   - initial phase. \n"
                    "  d   - duration of .wav file to be generated. \n"
                    "  w   - wave function (vals: `sine`, `sawtooth`, `triangle`, `noise`, `pulse`). \n"
    );
}


/**
 * \brief The function initializes the `audio_signal` structure.
 * 
 * \return Pointer to the `audio_signal` structure
 */
static audio_signal* parse_params(int argc, char** argv) {
    int index = 1;
    audio_signal *aud_signal = malloc(sizeof(audio_signal));
    aud_signal->carrier_params = malloc(sizeof(wave_params));

    aud_signal->out_file = argv[index++];
    aud_signal->carrier_params->amplitude = strtod(argv[index++], NULL);
    aud_signal->carrier_params->frequency = strtod(argv[index++], NULL);
    aud_signal->carrier_params->dutyCycle = strtod(argv[index++], NULL);
    aud_signal->carrier_params->sample_rate = aud_signal->sample_rate = atoi(argv[index++]);
    aud_signal->carrier_params->initial_phase = strtod(argv[index++], NULL);
    aud_signal->signal_duration = strtod(argv[index++], NULL);
    aud_signal->num_samples = aud_signal->signal_duration * aud_signal->sample_rate;
    aud_signal->samples = malloc(aud_signal->num_samples * sizeof(float));

    aud_signal->cnt_mrg_fn = argc - 8;
    aud_signal->merge_fn = malloc(aud_signal->cnt_mrg_fn * sizeof(wave_fn*));
    for (int i = 0; i < aud_signal->cnt_mrg_fn; i++) {
        aud_signal->merge_fn[i] = parse_wave_fn(argv[index++]);
    }

    return aud_signal;
}


int main(int argc, char** argv) {

    if (argc < 9) {
        help(argv[0]);
        exit(EXIT_FAILURE);
    }

    audio_signal *aud_signal = parse_params(argc, argv);
    gen_merge_wav(aud_signal);
    wr_wav_file(aud_signal);
    destructor(aud_signal);

    return 0;
}