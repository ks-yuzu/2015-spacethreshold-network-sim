#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include <cstdio>
#include <cassert>
#include "../define.h"


//enum PlotStyle { PLOT_LINES, PLOT_POINTS, PLOT_LINES_POINTS };


class Gnuplot
{
	public:
		Gnuplot();
		virtual ~Gnuplot();

		// operation
		void SetRange(double xMin, double xMax, double yMin, double yMax);
		void SetXRange(double min, double max);
		void SetYRange(double min, double max);
		void SetXLabel(const std::string xLabel);
		void SetYLabel(const std::string yLabel);
//		void SetPlotStyle(const PlotStyle ps){ plotStyle = ps; };
//		void SetPlotType(const PlotType pt)

		void SetLogScaleY(const bool fEnable = true);
		void SetLogScaleX(const bool fEnable = true);
		void SetLogScale(const bool fEnable = true);

		void SetGrid(const bool fEnable = true);

		void PlotXY(const std::vector<int>& x, const std::vector<int>& y);
		void Reset();
		void Execute();

	private:
		// const
		static const std::string endline;
		
		// operator
		template<class T> Gnuplot& operator<<(T val);


		// operation
		Gnuplot& command() { return *this; };

		// variable
		FILE *fp;
		std::stringstream buf;

		std::string title;

//		PlotStyle plotStyle;

};


template<typename T>
inline Gnuplot& Gnuplot::operator<<(T val)
{
	buf << val << " ";
	return *this;
}

//“ÁŽê‰»
template<>
inline Gnuplot& Gnuplot::operator<< <std::string>(std::string val)
{
	buf << val << " ";

	if( val == Gnuplot::endline ) { Execute(); }

	return *this;
}