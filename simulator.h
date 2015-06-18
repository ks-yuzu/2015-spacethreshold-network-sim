#pragma once

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

		void Draw(){ std::cout << "sim draw" << std::endl;}

	private:
		std::list<Node> nodes;		
};