//  localizationGlobalization.cpp

#include <vector>

#include "window.h"


/*	Compute partial sums and squared sums for all data	*/
void compute_sump_sumsq(short *data ,int count , std::vector<double> &sump , std::vector<double> &sumsq);

/*	Create window limits from seed	*/
Window create_window_from_seed (int seed);

/*	Grow window size	*/
void grow(Window &w,int count , bool &g);

/*	 Compute standard deviation and mean for given window	*/
void  compute_sd_m(Window &w,std::vector<double> const & sump , std::vector<double> const & sumsq);

/*	Compute localization windows for given data	*/
void compute_localization (short* data,int count, int channels , std::vector<Window> &loc );

/* Normalize */
short normalize (short* data,int count);

void denormalize (short* data,int count, short min);




bool areEqual (Window a , Window b);

bool mySortFunction ( Window a , Window b );

void getList (short * data , int count ,std::vector<Window> &loc , std::vector< std::vector<Window> > &rezult);


/*	Compute globalization windows for given data	*/
void compute_globalization (short *data,int count, int channels, std::vector<Window> &loc , std::vector<Window> &glob);

/*	Search for signal s in localization windows	*/
void search_for_s_in_loc(short *data ,int s , std::vector<Window> &loc,std::vector<Window> &rezult);
