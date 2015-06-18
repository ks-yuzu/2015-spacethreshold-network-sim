#pragma once

#include <iostream>
#include "node.h"

class Simulator
{
	public:
		Simulator()
		{
			nodes.push_back( Node(10, 10) );
		}
		virtual ~Simulator() = default;
//		void AppnedNodes(int num);

		void Draw();

	private:
		std::list<Node> nodes;		
};