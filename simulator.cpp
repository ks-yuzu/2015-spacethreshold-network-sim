#include <gl/glut.h>	
#include <gl/freeglut_ext.h>
#include "define.h"
#include "simulator.h"
#include "mtrand.h"
#include "fps.h"
#include "input/key.h"
#include "monitor/log_window.h"
#include "monitor/monitor_window.h"

//========================================
//              callback
//========================================
void Simulator::Draw()
{
	// vertex
	glColor4d(0.4, 0.9, 0.4, 0.2);
	for(Node *pNode1 : nodes)
	{
		for (Node *pNode2 : pNode1->Neighbors() )
		{
			Draw::Line( pNode1->GetPos(), pNode2->GetPos() );
		}
	}

	// node
	for each (Node *pNode in nodes) { pNode->Draw(); }
}

void Simulator::WindowResize(int w, int h)
{
	glViewport(0, 0, w, h);													// �E�B���h�E�S�̂��r���[�|�[�g��
	glLoadIdentity();														// �ϊ��s��̏�����
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);	//�X�N���[����̍��W�n���}�E�X�̍��W�n�Ɉ�v������
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
		Monitor::mout(0) << "node : " << nodes.size() << Command::endline;

		Monitor::AllWindowFlip();
		fps.Update();
		fps.Wait();
	}
}


void Simulator::Initialize()
{
	numNode = 0;
	for(Node *p : nodes){ delete p; } // �����d���@->�@�ʃX���b�h�ŉ���A�ʂ̃R���e�i���m��
	nodes.clear();

	AppnedNodes(standardNumNode);
	MakeEdge();

	// �O���t�ݒ�
	gnuplot.SetLogScale(); //���ΐ�
	gnuplot.SetXLabel("degree");
	gnuplot.SetYLabel("num of vertex");

	std::vector<int> x(standardNumNode);
	std::vector<int> degCount(standardNumNode);

	for(int i = 0; i < standardNumNode; i++) { x[i] = i; }
	for(Node *pNode : nodes) { ++degCount[ pNode->Neighbors().size() ]; }

//	SetRange(0, 1000, 0, 10000);
	gnuplot.PlotXY(x, degCount);


	// �������ꂢ��+�ʃv���Z�X �ړ� ��� �L�[�����i���ߓx�j�@�}���`�X���b�h
}


void Simulator::AppnedNodes(int num = standardNumNode)
{
	numNode += standardNumNode;
	nodes.reserve( numNode );

	for(int i = 0; i < num; i++) { nodes.push_back( new Node() ); }
}


void Simulator::MakeEdge()
{
	for(auto it1 = nodes.begin(); it1 != nodes.end(); it1++)
	{
		for(auto it2 = nodes.begin(); it2 != nodes.end(); it2++)
		{
			if( EdgeExists(**it1, **it2) ) { (*it1)->AddNeighbor( *it2 ); }
		}
	}
}


inline bool Simulator::EdgeExists(const Node& n1, const Node& n2) const
{
	// kuukann�@���f��
	return (&n1 != &n2) && (n1.Activity() + n2.Activity() > Node::maxActivity * 2 );
}


void Simulator::ProcInput()
{
	Keyboard& kb = Keyboard::GetInstance();

	if( kb('I') == 1) { Initialize(); }

	if( !kb(VK_SHIFT) )
	{
		const int moveDist = 20;
		Pos move(0, 0);
		if( kb(VK_LEFT)  ) { move += Pos( moveDist, 0); }
		if( kb(VK_RIGHT) ) { move += Pos(-moveDist, 0); }
		if( kb(VK_UP)    ) { move += Pos(0,  moveDist); }
		if( kb(VK_DOWN)  ) { move += Pos(0, -moveDist); }
		if( kb(VK_CONTROL) ) { move *= 3; }

		glTranslated(move.x, move.y, 0);
	}
	else if( kb(VK_UP)   ) { glScaled(1.02, 1.02, 1.02); }
	else if( kb(VK_DOWN) ) { glScaled(0.97, 0.97, 0.97); }
}