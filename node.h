#pragma once

#include <list>
#include <mutex>

#include "define.h"
#include "pos.h"
#include "draw.h"
#include "mtrand.h"
#include "hsv.h"
#include "monitor\log_window.h"

static const std::function<Pos()> posGen = 
	[]{
		auto rand = RandGen::unifi;
		return Pos( rand(0, mapSize.x), rand(0, mapSize.y) );
	};

class Node
{
	public:
	  // lifecycle
		Node(Pos pos = posGen()); // 位置指定なしで作成したノードは一様分布
		virtual ~Node();

	  // constant
		static const int maxActivity = 100;
		static const int minActivity = 0;
		static const int threshold = 30;//maxActivity * 2;

	  // operator
		bool operator==(const Node& n) const { return pos == n.pos; }

	  // accessor
		const Pos& GetPos()  const { return pos; }
		double Activity()    const { return activity; };
		size_t Degree()      const { return pNeighbors->size(); }

		std::vector<Node *>& Neighbors() const { return *pNeighbors; }
		void AddNeighbor(Node *neighbor) const;

	  // operation
		void Draw() const;
		static bool LinkExists(const Node& n1, const Node& n2);

	  // variable
		std::mutex *pNeighborsMtx;



	private:
	  // variable
		Pos pos;
		double activity;
		Rgbd color;
		std::vector<Node *> *pNeighbors;

	  // operator
		void AutoSetActivity();
};
