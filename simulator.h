#pragma once

#include <array>
#include <iostream>
#include "node.h"
#include "graph/gnuplot.h"


#include <vector>

class Simulator
{
	public:
		// lifecycle
		Simulator() : drawPos(0, 0), drawScale(1.0)
		{
			pCompleteRates = new std::array<double, 4>;
			pCompleteRates->fill(0.0);
			pCompleteRateMtx = new std::mutex;
		}
		virtual ~Simulator() = default;

		// callback
		void Draw();
		void WindowResize(int, int);

		// operations
		void MainLoop();
		void Initialize();

		// accessor
		double CompleteRate(int idx)           const { std::lock_guard<std::mutex> lock(*pCompleteRateMtx); return (*pCompleteRates)[idx]; }
		void CompleteRate(int idx, double val) const { std::lock_guard<std::mutex> lock(*pCompleteRateMtx); (*pCompleteRates)[idx] = val; }

	private:
		// variable
		int numNode;
		int numLink;
		std::vector<Node *> *pNodes;

		Pos drawPos;
		double drawScale;

		Gnuplot gnuplot;

		std::array<double, 4> *pCompleteRates;
		std::mutex *pCompleteRateMtx;

		// operation
		void AppnedNodes(int num = standardNumNode);
		void GenerateLink();      // space-threshold

		void ProcInput();
};