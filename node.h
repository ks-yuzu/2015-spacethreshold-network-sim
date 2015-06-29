#pragma once

#include <list>
#include <mutex>

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
			pNeighbors = new std::vector<Node *>;
			pMtx = new std::mutex;
			AutoSetActivity();
		}
		virtual ~Node() = default;

	  // constant
		static const int maxActivity = 100;
		static const int minActivity = 0;
		static const int threshold = 10;//maxActivity * 2;
	  // operator
		void Draw() const;

	  // accessor
		const Pos& GetPos() const { return pos; }
		const int Activity() const { return activity; };
		int Degree() const { return pNeighbors->size(); }

		std::vector<Node *>& Neighbors(){ return *pNeighbors; }
		void AddNeighbor(Node *neighbor) const { pNeighbors->push_back(neighbor); }

	  // operation
		static bool LinkExists(const Node& n1, const Node& n2)
		{
			auto sum = n1.Activity() * n2.Activity();
			sum = (int)( sum / Pos::distsq(n1.GetPos(), n2.GetPos()) );
			return sum > threshold;
		}

	private:
	  // variable
		Pos pos;
		int activity;
		Rgbd color;
		std::vector<Node *> *pNeighbors;

		std::mutex *pMtx;

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
	activity = (int)( 100 * prt(10, 10) );

	double colorRate = fmax(0, 1 - (double)activity / maxActivity);
	color = Hsvd2Rgbd((int)(240 * colorRate), 1, 1);
}