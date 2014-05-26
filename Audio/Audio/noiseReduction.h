// noiseReduction.cpp

#include "window.h"

#include <vector>
#include <iterator>


/** Median Filter **/
void medianfilter(short *data, int count, int channels ,std::vector< Window > &glob );

void sddevfilter(short *data, int count, int channels ,std::vector< Window > &win );