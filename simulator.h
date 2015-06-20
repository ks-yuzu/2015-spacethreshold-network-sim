#pragma once

#include <iostream>
#include "node.h"
#include "graph/gnuplot.h"

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
		std::list<Node> nodes;
		bool EdgeExists(const Node&, const Node&) const;

		Gnuplot gnuplot;

		// operation
		void AppnedNodes(int num);
		void MakeEdge();

		void ProcInput();
};