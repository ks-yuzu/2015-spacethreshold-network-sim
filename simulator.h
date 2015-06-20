#pragma once

#include <iostream>
#include "node.h"
#include "graph/pgnuplot.h"

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
		bool VertexExists(const Node&, const Node&) const;

		Gnuplot gnuplot;

		// operation
		void AppnedNodes(int num);
		void MakeVertex();

		void ProcInput();
};