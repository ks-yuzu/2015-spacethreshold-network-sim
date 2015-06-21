#pragma once

#include <string>
#include "pos.h"

static const std::string title = "MathPro_NetworkSimurator";

static const Pos windowSize(960, 672);
static const Pos windowPos(30, 20);
//static const int bitDepth		= 32;

static const Pos mapSize = windowSize;

static const int standardNumNode = 300;


static const std::string pathToGnuplot = "..\\tools\\gnuplot501\\bin\\gnuplot.exe";



// my math
#define M_PI 3.14159265358979323846
static const auto min = [](int a, int b){ return a < b ? a : b; };
