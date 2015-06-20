#pragma once

#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <cassert>


class Gnuplot
{
	public:
		Gnuplot();
		virtual ~Gnuplot();

	private:
		FILE *fp;

};


inline Gnuplot::Gnuplot()
{
	fp = _popen(pathToGnuplot.c_str() , "w");

	if( fp == NULL ) assert(0);

	fputs("plot sin(x)\n", fp);
	fflush(fp);

}

inline Gnuplot::~Gnuplot()
{
	std::cin.get();
	_pclose(fp);
}
