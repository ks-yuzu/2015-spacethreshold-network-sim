#pragma once

#include <string>
#include "pos.h"


// ----- System -----
constexpr int numThread = 4;

// ----- Window -----
static const std::string title = "MathPro_NetworkSimurator";
constexpr int windowHeight = 800;

constexpr Pos windowSize(windowHeight * 3 / 2, windowHeight);
constexpr Pos windowPos(30, 20);
//static const int bitDepth		= 32;



// ----- Simulator -----
constexpr int standardNumNode = 100000 *3;
constexpr Pos mapSize(standardNumNode * 3, standardNumNode * 2);


// ----- graph -----
static const std::string pathToGnuplot = "..\\tools\\gnuplot501\\bin\\gnuplot.exe";


// ----- math -----
constexpr double M_PI = 3.14159265358;

static std::string MakeProgBar(double rate, int len = 25)
{
	std::stringstream ss;
	ss << '[';
	for( int i = 0; i < len; ++i ) { ss << (i <= len * rate ? '*' : ' '); }
	ss << ']';
	return ss.str();
};
