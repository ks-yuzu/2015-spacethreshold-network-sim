#pragma once

#include <list>

#include "define.h"
#include "pos.h"
#include "draw.h"
#include "mtrand.h"

class Node
{
	public:
		Node();
		Node(int x, int y) : pos(x, y){}
		Node(Pos pos) : pos(pos){};
		virtual ~Node() = default;

		void Draw();

		// accessor
		const Pos& GetPos() const { return pos; }

		const std::list<Node *>& Neighbors(){ return neighbors; }
		void AddNeighbor(Node *pNode){ neighbors.push_back(pNode); }

	private:
		Pos pos;
		std::list<Node *> neighbors;
};


// 位置指定なしで作成したノードは一様分布
inline Node::Node()
: pos(0, 0)
{
	auto rand = RandGen::unif;
	pos = Pos( (int)rand(0, mapSize.x), (int)rand(0, mapSize.y) );
}

inline void Node::Draw()
{
	const int radius = 10;
	Draw::Circle(pos, radius, true);
}