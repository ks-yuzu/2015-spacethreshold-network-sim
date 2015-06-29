#pragma once

#include <iostream>
#include "node.h"
#include "graph/gnuplot.h"


#include <vector>

class Simulator
{
	public:
		// lifecycle
		Simulator() : drawPos(0, 0), drawScale(1.0) {}
		virtual ~Simulator() = default;

		// callback
		void Draw();
		void WindowResize(int, int);

		// operations
		void MainLoop();
		void Initialize();

	private:
		// variable
		int numNode;
		int numLink;
		std::vector<Node *> *pNodes;
		bool LinkExists(const Node*, const Node*) const;

		Pos drawPos;
		double drawScale;

		Gnuplot gnuplot;

		// operation
		void AppnedNodes(int num = standardNumNode);
		void GenerateLink();      // space-threshold

		void ProcInput();
};