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


// �ʒu�w��Ȃ��ō쐬�����m�[�h�͈�l���z
Node::Node()
: pos(0, 0)
{
	auto rand = RandGen::unif;
	pos = Pos( (int)rand(0, mapSize.x), (int)rand(0, mapSize.y) );
}