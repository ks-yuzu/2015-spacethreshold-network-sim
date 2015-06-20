#pragma once

#include <list>

#include "define.h"
#include "pos.h"
#include "draw.h"
#include "mtrand.h"
#include "hsv.h"


static const std::function<Pos()> posGen = []{
	auto rand = RandGen::unifi;
	return Pos( rand(0, mapSize.x), rand(0, mapSize.y) );
};

class Node
{
	public:
	  // lifecycle
		// 位置指定なしで作成したノードは一様分布
		Node(Pos pos = posGen()) : pos(pos), color(0, 0, 0){ AutoSetActivity(); };
		virtual ~Node() = default;

	  // constant
		static const int maxActivity = 100;
		static const int minActivity = 0;

	  // operator
		void Draw();
		void AddNeighbor(Node &node){ neighbors.push_back(&node); }

	  // accessor
		const Pos& GetPos() const { return pos; }
		const std::list<Node*> &Neighbors() const { return neighbors; }
		const int Activity() const { return activity; };

	private:
	  // val
		Pos pos;
		int activity;
		Rgbd color;
		std::list<Node*> neighbors;

	  // operator
		void AutoSetActivity();
};


inline void Node::Draw()
{
	const int radius = 5;

	glColor3d(color.r, color.g, color.b);
	Draw::Circle(pos, radius, true);
}


inline void Node::AutoSetActivity()
{
//	auto ur = RandGen::unifi;
//	activity = ur(minActivity, maxActivity);

	auto er = RandGen::exp;
	activity = (int)( 10 * er(0.4) );

	double colorRate = fmax(0, 1 - (double)activity / maxActivity);
	color = Hsvd2Rgbd((int)(240 * colorRate), 1, 1);

//	color = Rgbd(colorRate, fmin(colorRate, 1-colorRate)*2, 1-colorRate);

}