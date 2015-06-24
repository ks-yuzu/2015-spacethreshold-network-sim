#pragma once

#include <string>
#include "pos.h"

// ----- Window -----
static const std::string title = "MathPro_NetworkSimurator";
static const Pos windowSize(1200, 800);
static const Pos windowPos(30, 20);
//static const int bitDepth		= 32;


// ----- Simulator -----
static const int standardNumNode = 10000;
static const Pos mapSize = Pos(standardNumNode * 2, standardNumNode * 2);


// ----- graph -----
static const std::string pathToGnuplot = "..\\tools\\gnuplot501\\bin\\gnuplot.exe";


// ----- math -----
static const double M_PI = 3.14159265358;
static const auto min = [](int a, int b){ return a < b ? a : b; };
