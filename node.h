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
		static const int threshold = maxActivity * 2;
	  // operator
		void Draw() const;

	  // accessor
		const Pos& GetPos() const { return pos; }
		const int Activity() const { return activity; };
		const std::vector<Node *>::iterator& LeastNeighbor() const { return leastNeighbor; }
		void LeastNeighbor(std::vector<Node *>::iterator it) { leastNeighbor = it; }
		int Degree() const { return degree; }
		void Degree(int adeg) { degree = adeg; }

	private:
	  // variable
		Pos pos;
		int degree;
		int activity;
		Rgbd color;
		std::vector<Node *>::iterator leastNeighbor;

	  // operator
		void AutoSetActivity();
};


inline void Node::Draw() const
{
	const int radius = 5;

	glColor4d(color.r, color.g, color.b, 0.5);
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
}