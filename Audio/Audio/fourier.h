#ifndef AUDIO_FOURIER_H
#define AUDIO_FOURIER_H

#include <fftw3.h>
typedef fftw_complex audio_fourier_t;

#if 0
#include "kiss_fftr.h"
typedef kiss_fft_cpx audio_fourier_t;
#endif

void forwardFourier(short *data, int count, audio_fourier_t *transformedData);

void backwardFourier(audio_fourier_t *transformedData, int count, short *data);

void toReal(audio_fourier_t *transformedData, int count, short **realData);

void renormalize(audio_fourier_t *transformedData, int count, short *newReals, short *oldReals);

#endif //AUDIO_FOURIER_H
