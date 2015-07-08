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


// ----- Simulator -----
constexpr int standardNumNode = 100000;
constexpr Pos mapSize(standardNumNode * 3, standardNumNode * 2);


// ----- graph -----
static const std::string pathToGnuplot = "..\\tools\\gnuplot501\\bin\\gnuplot.exe";


// ----- math -----
constexpr double M_PI = 3.14159265358;

std::string MakeProgBar(double rate, int len = 25);
