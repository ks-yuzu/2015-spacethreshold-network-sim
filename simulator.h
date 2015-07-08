#pragma once

#include <array>
#include "node.h"
#include "graph/gnuplot.h"
#include "mutux-val.h"

#include <vector>

class Simulator
{
	public:
		// lifecycle
		Simulator();
		virtual ~Simulator() = default;

		// callback
		void Draw() const;
		void WindowResize(int, int);

		// operations
		void MainLoop();
		void Initialize();

		// accessor
		double CompleteRate(int idx)           const { std::lock_guard<std::mutex> lock(*pCompleteRateMtx); return (*pCompleteRates)[idx]; }
		void CompleteRate(int idx, double val) const { std::lock_guard<std::mutex> lock(*pCompleteRateMtx); (*pCompleteRates)[idx] = val; }

		// valriable
		MutexVal<int> mtNumLink;
		std::array<MutexVal<double>, numThread> mtCompleteRate;

	private:
		// variable
		int loopCount = 0;
		int numNode;
		int numLink;
		std::vector<Node *> *pNodes;

		std::array<double, 4> *pCompleteRates;
		std::mutex *pCompleteRateMtx;
			// -> MutexVal

		Pos drawPos;
		double drawScale;

		Gnuplot gnuplot;

		bool fGraphicalOut = true;

		// operation
		void AppnedNodes(int num = standardNumNode);
		void GenerateLink();

		void ProcInput();
		void DrawGraph();
		void MonitorOutput();
};