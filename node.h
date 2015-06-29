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
		Node(Pos pos = posGen()) : pos(pos), color(0, 0, 0)
		{
			neighbors = new std::vector<Node *>;
			AutoSetActivity();
		}
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
		int Degree() const { return degree; }
		void Degree(int adeg) { degree = adeg; }

		std::vector<Node *>& Neighbors(){ return *neighbors; }
		void AddNeighbor(Node *neighbor) const { neighbors->push_back(neighbor); }


	private:
	  // variable
		Pos pos;
		int degree;
		int activity;
		Rgbd color;
		std::vector<Node *> *neighbors;

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
//	auto& ur = RandGen::unifi;
//	activity = ur(minActivity, maxActivity);

//	auto& er = RandGen::exp;
//	activity = (int)( 100 * er(0.1) );

	auto& prt = RandGen::prt;
	activity = (int)( 100 * prt(5, 10) );

	double colorRate = fmax(0, 1 - (double)activity / maxActivity);
	color = Hsvd2Rgbd((int)(240 * colorRate), 1, 1);
}