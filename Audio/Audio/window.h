#include<vector>
#include <iterator>


#ifndef WINDOW_H
#define WINDOW_H

class Window {
	public :
		int begin;
		int end;

		Window ();
		~Window () {} ;
		Window (int , int);
		int getSize();
		double const getMean();
		double const getStDev();
		void setMean(double);
		void setStDev(double);
		
		/*	Print window information	*/
		void print();

	private :
		double sd;
		double m;
};

#endif