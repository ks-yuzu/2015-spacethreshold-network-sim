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

	double colorRate = min( max(0, 1 - activity / maxActivity), 1);
	color = Hsvd2Rgbd((int)(240 * colorRate), 1, 1);
}

void Node::AddNeighbor(Node *neighbor) const
{
	std::lock_guard<std::mutex> lock(*pNeighborsMtx);
	pNeighbors->push_back(neighbor);
}


void Node::Draw() const
{
	const int radius = static_cast<int>(4 * activity);

	glColor4d(color.r, color.g, color.b, 0.7);
	Draw::Circle(pos, radius, true);
}


bool Node::LinkExists(const Node& n1, const Node& n2)//, int debug_idx)
{
//	constexpr int threshold = 30;
	constexpr int threshold = 50; // nml-pos

	auto sum = n1.Activity() * n2.Activity();
//	sum *= 10;
//	sum /= pow(Pos::distsq(n1.GetPos(), n2.GetPos()), 1.0 / 3);
	sum *= 80;
	sum /= pow(Pos::distsq(n1.GetPos(), n2.GetPos()), 1.0 / 2);

//	auto sum = n1.Activity() + n2.Activity();
//	sum *= 100;
//	sum /= pow(  Pos::distsq(n1.GetPos(), n2.GetPos()), 1.0 / 3 );
//	sum *= 1000;
//	sum /= pow(Pos::distsq(n1.GetPos(), n2.GetPos()), 1.0 / 2);

//	Monitor::mout(debug_idx+1) << sum << Command::endline;

	//	Log::lout() << sum << Command::endline;
	return sum > threshold;
}
