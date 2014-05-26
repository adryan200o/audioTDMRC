#include "window.h"


Window::Window ()
{
	this->begin = 0 ;
	this->end = 0;
	this->m = 0;
	this->sd = 0;
}

Window::Window (int begin , int end)
{
	this->begin = begin ;
	this->end = end;
	this->m = 0;
	this->sd = 0;
}

int Window::getSize()
{
	return ( this->end - this->begin +1 ) ;
}

double const Window::getMean()
{
	return this->m;
}

double const Window::getStDev()
{
	return this->sd;
}

void Window::setMean(double m)
{
	this->m = m;
}

void Window::setStDev(double sd)
{
	this->sd = sd;
}

void Window::print()
{
	printf ("\tWindow info: begin: %i ; end: %i ; size: %i ; sd: %f ; m: %f \n", 
					this->begin , this->end , this->getSize(),this->getStDev() , this->getMean() );

	return ;
}
