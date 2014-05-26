#include "localizationGlobalization.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <vector>
#include <iterator>
#include <algorithm> 
#include <time.h>

/*	Compute partial sums and squared sums for all data	*/
void compute_sump_sumsq(short *data ,int count , std::vector<double> &sump , std::vector<double> &sumsq)
{
	if (count==0)
		printf ("Error - data is not valid\n");

	double prev_mean = 0 , m = 0 , n = 0 , s = 0 , sd = 0 , sum = 0 , sumq = 0;
	
	sump[0] = data[0];		
	sumsq[0] = pow(data[0], 2);

	for (int k = 1 ; k < count ; k++)
		{			
			//ecuatii squared sums
			sump[k] = sump[k-1] + data[k];			
			sumsq[k] = sumsq[k-1] + pow (data[k] , 2);			
		}
	return ;
}

/*	Create window limits from seed	*/
Window create_window_from_seed (int seed)
{
	return Window(seed , seed+1) ;
}

/*	Grow window size	*/
void grow(Window &w,int count , bool &g)
{//ar trebui verificata daca dimensiunea ferestrei este mai mare decat o dimensiune prestabilita nu daca ajunge la margine
	w.begin --;
	w.end ++;

	if ((w.end - w.begin) > 11025 * 0.1)
		g = false;

	if (w.begin < 0 && w.end > count-1)
	{
		g = false;
	}

	if(w.begin < 0){
		w.begin=0;
	}
	if(w.end > count-1){
		w.end=count-1;
	}
	/*
	if (w.begin == 0 )
		{
			if (w.end+1 <=count) //nu se deplaseaza fereastra deoarece e centrata in seed si se face un peding la inceput
				{
					w.end += 1;
					g = true;
					return;
				}
			else //
			{
				g= false;
				return;
			}
			
		}

	if (w.end == count-1)
		{
			if (w.begin-2 >= 0)
				{
					w.begin -= 2;
					g = true;
					return;
				}
			else
				{
					g=false;
					return;
				}
		}
	*/
	//aici nu ajunge niciodata = inutil
	/*
	if (g)
	{
		w.setMean(0);
		w.setStDev(0);
	}*/

	//g=false;
	//return;
}

/*	 Compute standard deviation and mean for given window	*/
void  compute_sd_m(Window &w,std::vector<double> const & sump , std::vector<double> const & sumsq)
{
	if (w.end <= w.begin)
		printf ("Error - window is not valid\n");
	
	double  m = 0 , sd = 0 , sum = 0 , sumq = 0 ;
	int n = 0;
	
	n = w.getSize()-1;

	int begin , end;

	if (w.begin > 0) 
		begin = w.begin - 1;
	else
		begin = w.begin;

	end = w.end;

	/*	Partial and squared sum for current window	*/
	sum = sump[ end ] - sump[ begin ];								
	sumq = sumsq[ end ] - sumsq[ begin ];							

	/*	Mean and standard deviation for current window	*/
	m = sum / n; 
	sd = sqrt(sumq/n-pow(sum/n, 2));//sqrt ( (sumq - ( pow(sum , 2) / n )) / (n-1) );


	w.setMean(m);
	w.setStDev(sd);
	
	return ;
}

/*	Compute localization windows for given data	*/
void compute_localization (short* data,int count, int channels , std::vector<Window> &loc )
{   
	printf ("compute_localization begin\n");

	/*	Parcurge daca sunt mai multe canale   */
	/*for (int chan = 0 ; chan < channels ; chan ++)  
	for (int k = chan ; k < count ; k+= channels)*/	
	
	std::vector<double> sump (count * channels , 0);  // partial sum vector
	std::vector<double> sumsq(count * channels , 0);  // squared sum vector
	
	compute_sump_sumsq(data , count , sump , sumsq);

	Window w_prev = Window();
	Window w_current = Window();
	Window w_first = Window();

	w_first = create_window_from_seed ( 0 );
	compute_sd_m( w_first , sump , sumsq );
	w_prev = w_first ;
	
	double med = 0;
	double w_size = 0 , w_max = DBL_MIN;

	//FILE * file;
	// file = fopen ("myfile.txt","w");

	for (int seed = 0 ; seed < count ; seed++)
	{
		w_current = create_window_from_seed (seed);
				
		while (1)
		{
			bool g=true;

			grow( w_current , count , g);

			med++;

			if ( !g )	/*	if ! grow	*/
				break;
			
			compute_sd_m( w_current ,sump , sumsq); 
			//printf(" %f",exp (w_current.getSize()));


			 

			// fprintf (file ,"%f\n", (w_current.getStDev()*log10 (w_current.getSize()) ));

			//if (w_current.getStDev()*w_current.getSize() < w_prev.getStDev()*w_prev.getSize())
			if (	w_current.getStDev()*log10 (w_current.getSize()) <	w_prev.getStDev()*log10(w_prev.getSize())  ) 
			//if (w_current.getStDev() <	w_prev.getStDev()) 
			{
				
				loc.push_back(w_current);//save current window

				w_size  += w_current.getSize();
				if (w_current.getSize() > w_max) w_max = w_current.getSize();

				w_prev = w_current;	
				break;
			}
						
			w_prev = w_current;
						
		}
						
	}

	printf ("\t Found windows : %i\n", loc.size() );
	
	printf ("\t While(1) med : %f\n", med/loc.size() );

	printf ("\t Window size med : %f\n", w_size/loc.size() );

	printf ("\t Window size max : %f\n", w_max);

	printf ("compute_localization end\n");

	//fclose (file);
	return ;
} /* compute_localization */


short normalize(short *data, int count){
	short min = SHRT_MAX;
	short max = SHRT_MIN;
	double sum = 0;

	for(int i=0; i<count; i++)
	{
		if(data[i] < min)
			min = data[i];
		if (abs (data[i]) > max)
			max = abs(data[i]);

		sum += data[i];

	}
	 //x_Norm = [x - min(x)] / [max(x) - min(x)];

	for(int i=0; i<count; i++)
	{
		data[i]  = (data[i] - min) ;

		//if(data[i] < 0) printf("\nEroare normalize\n");
	}

	return min;
}

void denormalize(short *data, int count, short min){
	for(int i=0; i<count; i++)
	{
		data[i]  = (data[i] + min) ;
	}
}





/*	Compute globalization windows for given data	*/
void compute_globalization (short *data,int count, int channels, std::vector<Window> &loc , std::vector<Window> &glob )
{
	printf ("compute_globalization begin\n");

	std::vector<Window> l ;
	std::vector< std::vector<Window> > list (count);
	std::vector<Window>::iterator it;

	clock_t t;
	t = clock();
		
		getList (data , count , loc , list);
	
	t = clock() - t;
	printf ("\t Compute time for finding loc windows for each signal : %.2f sec\n",((float)t)/CLOCKS_PER_SEC);
	
	int begin = 0;
	int end = count;
	int prag = count/10000;

	double max_size = 0 , size = 0 , intersection_size = 0;

	for (int s = 0 ; s < count ; s++)//data
	{		
		//printf ("s %i ",s);

		if ( s-prag > 0 )
			begin = s - prag;

		if ( s+prag < count )
			end = s +  prag;

		for (int ss = begin ; ss < end ; ss++)//data
		{
			
			
			
			//Intersectie var 1
			for (int o = 0 ; o <list[s].size() ; o++  ) //list for s
				for (int p = 0 ; p <list[ss].size() ; p++) //list for ss
				{
					if ( areEqual( list[s][o] , list[ss][p] ) )
					{
						l.push_back ( list[s][o] );
						
					}
				}
				
			/*//Intersectie var 2
			std::sort ( list [s].begin() ,list [s].end() , mySortFunction ); 
			std::sort ( list[ss].begin() ,list[ss].end() , mySortFunction ); 
			
			l.resize ( (int) ( list[s].size() + list[ss].size() ) );
		
			it = std::set_intersection ( list[s].begin() , list[s].end() , list[ss].begin() , list[ss].end() , l.begin() , mySortFunction );
			l.resize( it - l.begin() );*/

		}//ss for

		double max = DBL_MIN; //max for window sd		
		Window  max_w;
					
		intersection_size += l.size();
		for (int x = 0 ; x < l.size() ; x++  ) //l - intersection
		{				
			if ( l[x].getStDev() > max )
			{
				max = l[x].getStDev() ;
				max_w = l[x];  
			}								
		}

		/*std::sort ( l.begin() ,l.end() , mySortFunction ); 
		max = l[ l.size()-1 ].getStDev() ;
		max_w = l[ l.size()-1 ] ;*/
		
		if ( max != DBL_MIN )
		{
			glob.push_back ( max_w );

			if (max_w.getSize() > max_size) 
				max_size = max_w.getSize();

			size += max_w.getSize();

		}
		else
			printf ("\t Error - Globalization max not found\n");
			

		l.clear();
				
	}

	/*//Remove consecutive duplicates in range
	std::vector<Window>::iterator ig;
	ig = std::unique (glob.begin(), glob.end() ,areEqual);
	glob.resize( std::distance(glob.begin(),ig) );*/

	printf ("\t Found windows : %i\n", glob.size() );

	printf ("\t Intersection size med : %f\n", intersection_size/glob.size() );

	printf ("\t Window size med : %f\n", size/glob.size() );

	printf ("\t Window size max : %f\n", max_size );

	printf ("compute_globalization end\n");
	return;
}

bool mySortFunction ( Window a , Window b )
{ 
	return ( a.getStDev() < b.getStDev() );
}

bool areEqual (Window a , Window b)
{
	if ( a.begin == b.begin && 
		 a.end == b.end &&
		 a.getMean() == b.getMean() &&
		 a.getStDev() == b.getStDev() )

		 return 1 ;
	else
		return 0;
}

/*	Search for signal s in localization windows	*/
void search_for_s_in_loc(short *data ,int s , std::vector<Window> &loc ,std::vector<Window> &rezult)
{
	int begin = 0;
	int end = loc.size();

	int prag = loc.size()/1000;

	if ( s-prag > 0 )
		begin = s - prag;

	if ( s+prag < loc.size() )
		end = s +  prag;

	for (int w = begin ; w < end ; w++)//loc
	{
		if ( s >= loc[w].begin && s <= loc[w].end )
			rezult.push_back ( loc[w] ) ;
	}

	//Remove consecutive duplicates in range
	std::vector<Window>::iterator it;
	it = std::unique (rezult.begin(), rezult.end() ,areEqual);
	rezult.resize( std::distance(rezult.begin(),it) );

}

void getList (short * data , int count ,std::vector<Window> &loc , std::vector< std::vector<Window> > &rezult)
{
	std::vector<Window> aux;

	for (int i = 0 ; i < count ; i++)//data
	{
		search_for_s_in_loc(data , i ,loc, aux);

		rezult[i].resize( (int)aux.size() );

		std::copy ( aux.begin() , aux.end() , rezult[i].begin() );

		aux.clear();

	}

}


