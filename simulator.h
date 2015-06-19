#pragma once

#include <iostream>
#include "node.h"

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
		void AppnedNodes(int num);
		void MakeVertex();

	private:
		std::list<Node *> nodes;		
};