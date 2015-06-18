#pragma once

#include <list>

#include "define.h"
#include "pos.h"
#include "mtrand.h"

class Node
{
	public:
		Node();
		Node(int x, int y) : pos(x, y){}
		Node(Pos pos) : pos(pos){};
		virtual ~Node() = default;
//		void Draw();

	private:
		Pos pos;
		std::list<Node> neighbors;
};


// 位置指定なしで作成したノードは一様分布
Node::Node()
: pos(0, 0)
{
	auto rand = RandGen::unif;
	pos = Pos( (int)rand(0, mapSize.x), (int)rand(0, mapSize.y) );
}