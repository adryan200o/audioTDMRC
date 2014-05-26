#include "fourier.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <memory.h>

//se face transormata Fourier pe tot semnalul si se ia imaginar si real. se face conversia din cartezian in real.
//ne intereseaza amplitudinea. se trece cu filtrul peste el cu fereastra.

//putem considera imagine de 1pixel de 1024p.

//aplicam Fourier pe data si cu rezultatul aplicam conversia cartezian- real

void forwardFourier(short *data, int count, fftw_complex *transformedData)
{
	fftw_complex *fftData = fftw_alloc_complex(count);
	fftw_complex *tempData = fftw_alloc_complex(count);
	fftw_plan p;

	for (int  i = 0; i < count; i++)
	{
		fftData[i][0] = (double)data[i];
		fftData[i][1] = 0;
	}

	p = fftw_plan_dft_1d(count, fftData, tempData, FFTW_FORWARD, FFTW_ESTIMATE/* | FFTW_UNALIGNED*/);
	fftw_execute(p);
	fftw_destroy_plan(p);

	memcpy(transformedData, tempData, sizeof(fftw_complex) * count);
	
	fftw_free(fftData);
	fftw_free(tempData);
}

void backwardFourier(fftw_complex *transformedData, int count, short *data)
{
	fftw_complex *fftData = new fftw_complex[count];
	fftw_plan p;

	p = fftw_plan_dft_1d(count, transformedData, fftData, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_UNALIGNED);
	fftw_execute(p);
	fftw_destroy_plan(p);

	for (int i = 0; i < count; i++)
		data[i] = fftData[i][0];
		//(*data)[i] = sqrt(pow(fftData[i][0], 2) + pow(fftData[i][1], 2));
	
	delete fftData;
}

void toReal(fftw_complex *transformedData, int count, short **realData)
{
	*realData = new short[count];

	for (int i = 0; i < count; i++)
	{
		if (sqrt(pow(transformedData[i][0], 2) + pow(transformedData[i][1], 2))/300 > SHRT_MAX)
			printf("overflow\n");
		(*realData)[i] = sqrt(pow(transformedData[i][0], 2) + pow(transformedData[i][1], 2)) / 300;
	}
}

void renormalize(fftw_complex *transformedData, int count, short *newReals, short *oldReals)
{
	for (int i = 0; i < count; i++)
	{
		if (!oldReals[i])
			continue;

		transformedData[i][0] *= newReals[i] / oldReals[i];
		transformedData[i][1] *= newReals[i] / oldReals[i];
	}
}

#if 0
void forwardFourier(short *data, int count, kiss_fft_cpx **transformedData)
{
	kiss_fft_scalar *fftData = new kiss_fft_scalar[count];
	kiss_fftr_cfg cfg;
	
	*transformedData = new kiss_fft_cpx[count];

	for (int  i = 0; i < count; i++)
		fftData[i]= ((double)data[i] - SHRT_MIN) / USHRT_MAX;

	cfg = kiss_fftr_alloc(count, 0, NULL, NULL);
	if (!cfg)
	{
		fprintf(stderr, "oom\n");
		system("pause");
		exit(1);
	}

	kiss_fftr(cfg, fftData, *transformedData);
    free(cfg);

	delete fftData;
}

void backwardFourier(kiss_fft_cpx *transformedData, int count, short **data)
{
	kiss_fft_scalar *fftData = new kiss_fft_scalar[count];
	printf("fftData = %p\n", fftData);
	kiss_fftr_cfg cfg;

	*data = new short[count];
	
	cfg = kiss_fftr_alloc(count, 1, NULL, NULL);
	if (!cfg)
	{
		fprintf(stderr, "oom\n");
		system("pause");
		exit(1);
	}
	kiss_fftri(cfg, transformedData, fftData);
    free(cfg);

	for (int i = 0; i < count; i++)
		(*data)[i] = fftData[i]  * USHRT_MAX + SHRT_MIN;
	
	printf("fftData = %p\n", fftData);
	delete fftData;
}

void toReal(kiss_fft_cpx *transformedData, int count, short **realData)
{
	*realData = new short[count];

	for (int i = 0; i < count; i++)
	{
		if (sqrt(pow(transformedData[i].r, 2) + pow(transformedData[i].i, 2))/300 > SHRT_MAX)
			printf("overflow\n");
		(*realData)[i] = sqrt(pow(transformedData[i].r, 2) + pow(transformedData[i].i, 2)) / 300;
	}
}

void renormalize(kiss_fft_cpx *transformedData, int count, short *newReals, short *oldReals)
{
	for (int i = 0; i < count; i++)
	{
		if (!oldReals[i])
			continue;

		transformedData[i].r *= newReals[i] / oldReals[i];
		transformedData[i].i *= newReals[i] / oldReals[i];
	}
}
#endif