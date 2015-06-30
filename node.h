#pragma once

#include <list>
#include <mutex>

#include "define.h"
#include "pos.h"
#include "draw.h"
#include "mtrand.h"
#include "hsv.h"
#include "monitor\log_window.h"

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
		static const int threshold = 3;//maxActivity * 2;

	  // operator
		bool operator==(const Node& n) const { return pos.x == n.pos.x && pos.y == n.pos.y; }

	  // accessor
		const Pos& GetPos()     const { return pos; }
		const double Activity() const { return activity; };
		int Degree()            const { return pNeighbors->size(); }

		std::vector<Node *>& Neighbors(){ return *pNeighbors; }
		void AddNeighbor(Node *neighbor) const
		{
			std::lock_guard<std::mutex> lock(*pMtx);
			pNeighbors->push_back(neighbor);
		}

	  // operation
		void Draw() const;

		static bool LinkExists(const Node& n1, const Node& n2)
		{
			auto sum = n1.Activity() * n2.Activity();
			sum *= (long long int)standardNumNode * standardNumNode;
			sum /= Pos::distsq(n1.GetPos(), n2.GetPos());
//			Log::lout() << sum << Command::endline;
			return sum > threshold;
		}

	private:
	  // variable
		Pos pos;
		double activity;
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
	activity = prt(4, 10);

//	Log::lout() << activity << Command::endline;

	double colorRate = fmax(0, 1 - (double)activity / maxActivity);
	color = Hsvd2Rgbd((int)(240 * colorRate), 1, 1);
}