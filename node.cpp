#include "node.h"


// 位置指定なしで作成したノードは一様分布
Node::Node(Pos pos) : pos(pos), color(0, 0, 0)
{
	pNeighbors = new std::vector<Node *>;
	pNeighborsMtx = new std::mutex;
	AutoSetActivity();
}

Node::~Node()
{
	delete pNeighbors;
	delete pNeighborsMtx;
}


void Node::AutoSetActivity()
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

void Node::AddNeighbor(Node *neighbor) const
{
	std::lock_guard<std::mutex> lock(*pNeighborsMtx);
	pNeighbors->push_back(neighbor);
}


void Node::Draw() const
{
	const int radius = 5;

	glColor4d(color.r, color.g, color.b, 0.5);
	Draw::Circle(pos, radius, true);
}


bool Node::LinkExists(const Node& n1, const Node& n2)
{
	auto sum = n1.Activity() * n2.Activity();
	sum *= (long long int)standardNumNode;// * standardNumNode;
	sum /= Pos::distsq(n1.GetPos(), n2.GetPos());
//	Log::lout() << sum << Command::endline;
	return sum > threshold;
}
