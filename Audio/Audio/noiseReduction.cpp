#include "noiseReduction.h"
#include "localizationGlobalization.h"

#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <limits.h>

void sddevfilter(short *data, int count, int channels ,std::vector< Window > &win )
{
	int axed = 0;

	printf ("sddevfilter begin\n");

	for(int i = 0; i < win.size() ; i++)
	{
		int index = data[win[i].begin + win[i].getSize() / 2];
		if (data[index] < win[i].getStDev())
		{
			data[index] = 0;
			axed++;
		}
	}

	printf ("sddevfilter end axed %d\n",axed);
	return;
}

/** Median Filter - Localization **/                       
void medianfilter(short *data, int count, int channels ,std::vector< Window > &win )
{
   printf ("median_filter begin\n");
   std::vector<short> data_copy(data, data + count);
   
   for(int i = 0; i < win.size() - 1 ; i++)//glob
	{
	   std::vector<short> aux;

	   for(int j = win[i].begin; j < win[i].end; j++)
		   aux.push_back( data_copy[j] );
		  
	   sort ( aux.begin() , aux.end() );
	   
	   data[i] = aux[win[i].getSize()/2];
	}

   printf ("median_filter end\n");
   return;
}

/** Average Filter - Localization **/                       
void medianfilter1(short *data, int count, int channels ,std::vector< Window > &win )
{
   printf ("median_filter begin\n");
   
   std::vector<short> data_copy(data, data + count);

   for(int i = 0; i < win.size() ; i++)//glob
	{
	   short min = SHRT_MAX;
	   for(int j = win[i].begin; j < win[i].end; j++) {
		   if (data_copy[j] < min) {
				min = data_copy[j];
		   }
	   }
	   long sum = 0;
	   for(int j = win[i].begin; j < win[i].end; j++)
		   sum += data_copy[j] - min;
	   
	   data[i] = sum / win[i].getSize() + min;
	}

   printf ("median_filter end\n");
   return;
}

/** Median Filter - fixed window **/                       
void medianfilter3(short *data, int count, int channels ,std::vector< Window > &win ) {
   printf ("median_filter begin\n");
   
   std::vector<short> data_copy(data, data + count);

   for(int i = 0; i < count ; i++)//glob
	{
	   int width = 2;
	   int start = std::max(0, i - width);
	   int finish = std::min(count, i + width + 1);
	   int length = finish - start;

	   std::vector<short> aux;

	   for(int j = start; j < finish; j++)
		   aux.push_back( data_copy[j] );
		  
	   sort ( aux.begin() , aux.end() );
	   
	   data[i] = aux[length/2];
	}

   printf ("median_filter end\n");
   return;
}

/** Average Filter - fixed window **/                       
void medianfilter6(short *data, int count, int channels ,std::vector< Window > &win ) {
   printf ("median_filter begin\n");
   
   std::vector<short> data_copy(data, data + count);

   for(int i = 0; i < count ; i++)//glob
	{
	   int width = 2;
	   int start = std::max(0, i - width);
	   int finish = std::min(count, i + width + 1);
	   int length = finish - start;

	   short min = SHRT_MAX;
	   for(int j = start; j < finish; j++) {
		   if (data_copy[j] < min) {
				min = data_copy[j];
		   }
	   }

	   long sum = 0;
	   for(int j = start; j < finish; j++)
		   sum += data_copy[j] - min;
	   
	   data[i] = sum / length + min;
	}

   printf ("median_filter end\n");
   return;
}
