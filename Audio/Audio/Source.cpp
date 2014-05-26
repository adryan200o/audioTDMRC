#include "localizationGlobalization.h"
#include "adaptiveAmplification.h"
#include "noiseReduction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include <math.h>
#include <time.h> 
#include <vector>
#include <iterator>
#include <algorithm> 

#include "fourier.h"


#define MAX_CHANNELS    6

static const int CHUNK_SIZE = 2;

int main (void)
{   
	
    SNDFILE      *infile, *outfile_nr , *outfile_nor  , *outfile_aa ,*outfile_f; /* input , output for noise reduction ,output normalized output for adaptive amplification*/
    SF_INFO      sfinfo ;
    int          readcount ;


   // if (! (infile = sf_open ("adios.wav",SFM_READ, &sfinfo)))
	if (! (infile = sf_open ("lion.wav",SFM_READ, &sfinfo)))
    {   /* Open failed so print an error message. */
        printf ("Not able to open input file %s.\n", "input.wav") ;
        /* Print the error message fron libsndfile. */
        sf_perror (NULL) ;
		system("pause");
        return  1 ;
        } ;

	if (sfinfo.channels > MAX_CHANNELS)
    {   printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
        return  1 ;
        } ;
	
	int size = sfinfo.frames * sfinfo.channels;
	const int in_frames = sfinfo.frames;

	short *data;
	data = (short*) malloc(size*sizeof (short));	

	std::vector<Window> loc ;
	std::vector<Window> glob ;	

	printf ("Input has %d channels\n", sfinfo.channels);
	printf ("Input has %i frames\n\n",sfinfo.frames);
	

    if (! (outfile_nr = sf_open ("output_nr.wav",SFM_WRITE, &sfinfo)))
    {   printf ("Not able to open output for noise reduction file %s.\n", "output_nr.wav") ;
        sf_perror (NULL) ;
        return  1 ;
        } ;

	 if (! (outfile_f = sf_open ("output_f.wav",SFM_WRITE, &sfinfo)))
    {   printf ("Not able to open output for noise reduction file %s.\n", "output_f.wav") ;
        sf_perror (NULL) ;
        return  1 ;
        } ;

	if (! (outfile_nor = sf_open ("output_nor.wav",SFM_WRITE, &sfinfo)))
    {   printf ("Not able to open output for noise reduction file %s.\n", "output_nor.wav") ;
        sf_perror (NULL) ;
        return  1 ;
        } ;

	 if (! (outfile_aa = sf_open ("outfile_aa.wav",SFM_WRITE, &sfinfo)))
    {   printf ("Not able to open output for adaptive amplification file %s.\n", "outfile_aa.wav") ;
        sf_perror (NULL) ;
        return  1 ;
        } ;


	clock_t t;

	int contor = 0;
	/* While there are samples in the input file, read them and process them */
	audio_fourier_t *transformedData = new audio_fourier_t[in_frames];
	audio_fourier_t *transformedDataCrt = transformedData;
	short *dataCrt = data;

	while ((readcount = sf_readf_short (infile, dataCrt, CHUNK_SIZE)))
    {
		contor++;
		forwardFourier(dataCrt, readcount, transformedDataCrt);
		transformedDataCrt += CHUNK_SIZE;
		dataCrt += CHUNK_SIZE/2;
    }
	short *realData, *oldRealData;
	short *newData = new short[in_frames];

	{
		
		
		toReal(transformedData, in_frames, &realData);
		toReal(transformedData, in_frames, &oldRealData);


		/** Localization **/
		t = clock();
		compute_localization (realData , in_frames, sfinfo.channels , loc) ;
		t = clock() - t;
		printf ("Compute time for Localization : %.2f seconds\n\n",((float)t)/CLOCKS_PER_SEC);
		
		
		/** Globalization **/
		t = clock();
		//compute_globalization (data, in_frames, sfinfo.channels,  loc , glob );
		t = clock() - t;
		printf ("Compute time for Globalization : %.2f seconds\n",((float)t)/CLOCKS_PER_SEC);
		printf ("Compute time for Globalization : %.2f minutes\n\n",((float)t)/CLOCKS_PER_SEC/60);

		
		
		/** Median Filter **/
		t = clock();
		//sddevfilter(realData, in_frames, sfinfo.channels , loc );
		medianfilter(realData, in_frames, sfinfo.channels, loc);
		

		
	}
	
	/* fisierul nemodificat */
	for (int i = 0; i < in_frames / CHUNK_SIZE; i++)
	{
		backwardFourier(transformedData + i * CHUNK_SIZE, CHUNK_SIZE, newData + i * CHUNK_SIZE);
	}
	sf_writef_short (outfile_f, newData, in_frames) ;

	/* filtrul aplicat */
	renormalize(transformedData, in_frames, realData, oldRealData);
	for (int i = 0; i < in_frames / CHUNK_SIZE; i++)
	{
		backwardFourier(transformedData + i * CHUNK_SIZE, CHUNK_SIZE, newData + i * CHUNK_SIZE);
	}
	
	sf_writef_short (outfile_nr, newData, in_frames) ;
	t = clock() - t;
	printf ("Compute time for Median Filter : %.2f seconds\n\n",((float)t)/CLOCKS_PER_SEC);


	printf("Contor read: %d\n", contor * CHUNK_SIZE);

    /* Close input and output files. */
    sf_close (infile) ;
    sf_close (outfile_nr) ;
	sf_close (outfile_nor) ;
	sf_close (outfile_aa) ;
	sf_close (outfile_f) ;

	printf("\n\n\n");
	system("pause");
    return 0 ;
} /* main */

