#pragma once

#include <string>
#include "pos.h"

// ----- System -----
static const int numThread = 4;

// ----- Window -----
static const std::string title = "MathPro_NetworkSimurator";
constexpr Pos windowSize(1200, 800);
constexpr Pos windowPos(30, 20);
//static const int bitDepth		= 32;



// ----- Simulator -----
static const int standardNumNode = 50000;
static const Pos mapSize = Pos(standardNumNode * 3, standardNumNode * 2);


// ----- graph -----
static const std::string pathToGnuplot = "..\\tools\\gnuplot501\\bin\\gnuplot.exe";


// ----- math -----
static const double M_PI = 3.14159265358;
static const auto min = [](int a, int b){ return a < b ? a : b; };
static const auto max = [](int a, int b){ return a > b ? a : b; };


static std::string MakeProgBar(double rate, int len = 25)
{
	std::stringstream ss;
	ss << '[';
	for(int i = 0; i < len; ++i) { ss << (i <= len * rate ? '*' : ' '); }
	ss << ']';
	return ss.str();
};
