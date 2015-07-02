#include "gnuplot.h"

Gnuplot::Gnuplot()
{
	fp = _popen(pathToGnuplot.c_str() , "w");

	if( fp == nullptr )
	{
		std::cerr << "popen error(gnuplot)" << std::endl;
		exit(EXIT_FAILURE);
	}
}


Gnuplot::~Gnuplot()
{
	_pclose(fp);
}



void Gnuplot::SetRange(double xMin, double xMax, double yMin, double yMax)
{
	SetXRange(xMin, xMax);  SetYRange(yMin, yMax);
}

void Gnuplot::SetXRange(double min, double max)
{
	command() << "set xrange [" << min << ":" << max << "]" << endline;
}

void Gnuplot::SetYRange(double min, double max)
{
	command() << "set yrange [" << min << ":" << max << "]" << endline;
}



void Gnuplot::SetXLabel(std::string xLabel)
{
	command() << "set xlabel '" << xLabel << "'" << endline;
}

void Gnuplot::SetYLabel(std::string yLabel)
{
	command() << "set ylabel '" << yLabel << "'" << endline;
}



void Gnuplot::SetLogScaleX(const bool fEnable)
{
	if(fEnable)
	{
		command() << "set logscale x" << endline;
		command() << "set format x \"10^{%%L}\"" << endline;
	} else {
		command() << "set nolog x" << endline;
		command() << "set format x" << endline;
	}
}

void Gnuplot::SetLogScaleY(const bool fEnable)
{
	if(fEnable)
	{
		command() << "set logscale y" << endline;
		command() << "set format y \"10^{%%L}\"" << endline;
	} else {
		command() << "set nolog y" << endline;
		command() << "set format y" << endline;
	}
}

void Gnuplot::SetLogScale(const bool fEnable)
{
	SetLogScaleX(fEnable);
	SetLogScaleY(fEnable);
}


void Gnuplot::SetGrid(const bool fEnable)
{
	command() << (fEnable ? "set grid" : "set nogrid") << endline;
}


void Gnuplot::PlotXY(const std::vector<int>& x, const std::vector<int>& y)
{
	std::stringstream buf;
	command() << "plot '-'" << "with points" << "pointtype 1" << "title \"test\"" << endline;
//	command() << buf.str();
//	Execute();

	size_t last = min( x.size(), y.size() );
	for(size_t i = 0; i < last; i++)
	{
		command() << x[i] << " " << y[i] << endline;    // ƒf[ƒ^‚Ì‘‚«ž‚Ý
	}  

	command() << "e" << endline;

	fflush(fp);
}



void Gnuplot::Reset(){ command() << "reset" << endline; }

void Gnuplot::Execute()
{
	fprintf(fp, buf.str().c_str());
	buf.str("");
	buf.clear(std::stringstream::goodbit);
}

//===================================
//              PRIVETE
//===================================

const std::string Gnuplot::endline = "\n";


