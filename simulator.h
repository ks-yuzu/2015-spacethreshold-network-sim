#pragma once

#include <iostream>
#include "node.h"
#include "graph/gnuplot.h"


#include <vector>

class Simulator
{
	public:
		// lifecycle
		Simulator(){}
		virtual ~Simulator() = default;

		// callback
		void Draw();
		void WindowResize(int, int);

		// operations
		void MainLoop();
		void Initialize();

	private:
		// val
		int numNode;
		std::vector<Node *> *pNodes;
		bool EdgeExists(const Node&, const Node&) const;

		Gnuplot gnuplot;

		// operation
		void AppnedNodes(int num = standardNumNode);
		void GenerateLink();

		void ProcInput();
};