#include <algorithm>
#include <thread>
#include <gl/glut.h>	
#include <gl/freeglut_ext.h>
#include "define.h"
#include "simulator.h"
#include "mtrand.h"
#include "fps.h"
#include "input/key.h"
#include "monitor/log_window.h"
#include "monitor/monitor_window.h"
#include "deleter.h"


//========================================
//              callback
//========================================
void Simulator::Draw()
{
  // vertex
	glColor4d(0.4, 0.9, 0.4, 0.2);

	// i : iterator,  p : pointer
	auto ipEnd = std::end(*pNodes);
	for(Node *pNode : *pNodes)
	{
		for(auto pNeighbor : pNode->Neighbors())
		{
			Draw::Line( pNode->GetPos(), pNeighbor->GetPos() );
		}
	}

  // node
	for each (Node *pNode in *pNodes) { pNode->Draw(); }
}

void Simulator::WindowResize(int w, int h)
{
	glViewport(0, 0, w, h);													// �E�B���h�E�S�̂��r���[�|�[�g��
	glLoadIdentity();														// �ϊ��s��̏�����
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);	// �X�N���[����̍��W�n���}�E�X�̍��W�n�Ɉ�v������
}


//========================================
//              operator
//========================================

void Simulator::MainLoop()
{
	Keyboard& kb = Keyboard::GetInstance();
	FpsControl& fps = FpsControl::GetInstance();
//	Log::cout.Init();
//	Log::cout << "Log Window" << Command::endline << Command::endline; //���O�E�B���h�E�̐ݒ�ƃ^�C�g��


	while( fps.SetStartTime(), kb.Update() )
	{
		// �C�x���g����
		glutMainLoopEvent();

		// �L�[���͏���
		ProcInput();

		// �`��C�x���g�Ăяo��
		glutPostRedisplay();	

		// ���[�v������
		Monitor::mout(0) << Command::endline;
		Monitor::mout(0) << "node  : " << numNode << Command::endline;
		Monitor::mout(0) << "link  : " << numLink << Command::endline;

		Monitor::mout(0) << Command::endline;
		Monitor::mout(0) << "pos   : " << drawPos << Command::endline;
		Monitor::mout(0) << "scale : " << drawScale << Command::endline;

		Monitor::AllWindowFlip();
		fps.Update();
		fps.Wait();
	}
}

void Simulator::Initialize()
{
	numNode = 0;

	// �ʃX���b�h�ŉ��
	if( pNodes != nullptr ) { Deleter<Node> deleter(pNodes); }

	pNodes = new std::vector<Node *>;
	AppnedNodes(standardNumNode);
	GenerateLink();

	// �O���t�ݒ�
	gnuplot.SetLogScale(); //���ΐ�
	gnuplot.SetXLabel("degree");
	gnuplot.SetYLabel("num of vertex");

	std::vector<int> x(standardNumNode);
	std::vector<int> degCount(standardNumNode);

	for(int i = 0; i < standardNumNode; i++) { x[i] = i; }
	for(Node *pNode : *pNodes) { ++degCount[ pNode->Degree() ]; }

//	SetRange(0, 1000, 0, 10000);
	gnuplot.PlotXY(x, degCount);

}


void Simulator::AppnedNodes(int num)
{
	numNode += standardNumNode;
	pNodes->reserve( numNode );

	for(int i = 0; i < num; i++) { pNodes->push_back( new Node() ); }

	std::sort(std::begin(*pNodes), std::end(*pNodes), [](Node *n1, Node *n2){ return n1->Activity() < n2->Activity(); });
}


void Simulator::GenerateLink()
{
	numLink = 0;

	const auto ipBegin = std::begin(*pNodes), ipEnd = std::end(*pNodes);

	for(auto ipNode1 = ipBegin; ipNode1 != ipEnd; ++ipNode1)
	{
		for(auto ipNode2 = ipNode1+1; ipNode2 != ipEnd; ++ipNode2)
		{
			if( LinkExists(*ipNode1, *ipNode2) )
			{
				(*ipNode1)->AddNeighbor(*ipNode2);
				(*ipNode2)->AddNeighbor(*ipNode1);
			}
		}

		int deg = (*ipNode1)->Neighbors().size();
		(*ipNode1)->Degree( deg );
		numLink += deg;
	}
}


bool Simulator::LinkExists(const Node *pn1, const Node *pn2) const
{
	auto sum = pn1->Activity() * pn2->Activity();
	sum /= Pos::dist(pn1->GetPos(), pn2->GetPos());
	return sum > 5;//Node::threshold;
}


void Simulator::ProcInput()
{
	Keyboard& kb = Keyboard::GetInstance();

	if( kb('I') == 1) 
		{ Initialize(); }

	if( !kb(VK_SHIFT) )
	{
		const int moveDist = 20;
		Pos move(0, 0);
		if( kb(VK_LEFT)  ) { move += Pos( moveDist, 0); }
		if( kb(VK_RIGHT) ) { move += Pos(-moveDist, 0); }
		if( kb(VK_UP)    ) { move += Pos(0,  moveDist); }
		if( kb(VK_DOWN)  ) { move += Pos(0, -moveDist); }
		if( kb(VK_CONTROL) ) { move *= 3; }

		drawPos -= move;
		glTranslated(move.x, move.y, 0);
	}
	else if( kb(VK_UP)   ) { drawScale *= 1.02; glScaled(1.02, 1.02, 1.02); }
	else if( kb(VK_DOWN) ) { drawScale *= 0.97; glScaled(0.97, 0.97, 0.97); }
}