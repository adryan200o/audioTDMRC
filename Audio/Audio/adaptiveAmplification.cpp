#include "adaptiveAmplification.h"
#include "window.h"

#include <stdio.h>


/*	Compute adaptive amplification for given data	*/
void compute_adaptive_amplification(short *data,int count, int channels , Window * loc ,  int & nr_loc)
{
	printf ("\t\t compute_adaptive_amplification begin\n");
	//	Parcurge daca sunt mai multe canale   
	//for (int chan = 0 ; chan < channels ; chan ++)  
	//for (int k = chan ; k < count ; k+= channels)

	for (int i = 0 ; i < count ; i++)
	{
		//process data[i]
		//TO DO 
	}

	printf ("\t\t compute_adaptive_amplification end\n");
	return;
}