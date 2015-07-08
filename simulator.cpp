#include <thread>
#include <gl/glut.h>	
#include <gl/freeglut_ext.h>

#include "define.h"
#include "simulator.h"
#include "fps.h"
#include "input/key.h"
#include "monitor/log_window.h"
#include "monitor/monitor_window.h"
#include "deleter.h"
#include "mtrand.h"


Simulator::Simulator()
: drawPos(0, 0), drawScale((double)windowHeight / (standardNumNode * 2))
{
	standardNumNode;
	pCompleteRates = new std::array<double, 4>;
	pCompleteRates->fill(0.0);
	pCompleteRateMtx = new std::mutex;

	mtNumLink(0);
}



//========================================
//              callback
//========================================
void Simulator::Draw() const
{
	if( !fGraphicalOut ) { return; }

	glPushMatrix();
	glTranslated(drawPos.x, drawPos.y, 0);
	glScaled(drawScale, drawScale, drawScale);

	// --- �`�揈���J�n ---

	// vertex
	glColor4d(0.4, 0.9, 0.4, 0.2);

	std::vector<int> vertexes;
	vertexes.reserve(standardNumNode*10);

	for(Node *pNode : *pNodes)
	{
		for(const auto pNeighbor : pNode->Neighbors())
		{
			vertexes.push_back(pNode->GetPos().x);
			vertexes.push_back(pNode->GetPos().y);

			vertexes.push_back(pNeighbor->GetPos().x);
			vertexes.push_back(pNeighbor->GetPos().y);
		}
	}

	Draw::Lines(vertexes.data(), vertexes.size() / 2, 2);

	// node
	for each (Node *pNode in *pNodes) { pNode->Draw(); }
	
	// --- �`�揈�������܂� ---
	glPopMatrix();
}

void Simulator::WindowResize(int w, int h)
{
	glViewport(0, 0, w, h);													// �E�B���h�E�S�̂��r���[�|�[�g��
	glLoadIdentity();														// �ϊ��s��̏�����
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);	// �X�N���[����̍��W�n���}�E�X�̍��W�n�Ɉ�v������
}


//========================================
//               MainLoop
//========================================

void Simulator::MainLoop()
{
	Keyboard& kb = Keyboard::GetInstance();
	FpsControl& fps = FpsControl::GetInstance();

	while( fps.SetStartTime(), kb.Update() )
	{
		// �C�x���g����
		glutMainLoopEvent();

		// �L�[���͏���
		ProcInput();

		// �`��C�x���g�Ăяo��
		glutPostRedisplay();

		// ���[�v������
		MonitorOutput();
		Monitor::AllWindowFlip();
		fps.Update();
		fps.Wait();
	}
}


//========================================
//             �������֘A
//========================================

void Simulator::Initialize()
{
	// �ʃX���b�h�ŉ��
	if( pNodes != nullptr ) { Deleter<Node> deleter(pNodes); }

	numNode = 0;
	mtNumLink(0);

	pNodes = new std::vector<Node *>;
	AppnedNodes(standardNumNode);
	GenerateLink();
}


// -------------------- �m�[�h���� --------------------
/*
// ��l���z
void Simulator::AppnedNodes(int num)
{
	numNode += standardNumNode;
	pNodes->reserve( numNode );

	for(int i = 0; i < num; i++) { pNodes->push_back( new Node() ); }

//	std::sort(std::begin(*pNodes), std::end(*pNodes), [](Node *n1, Node *n2){ return n1->Activity() < n2->Activity(); });
}
*/


// �m�[�h���V�[�h�𒆐S�Ƃ������K���z������
void Simulator::AppnedNodes(int num)
{
	constexpr int dev = standardNumNode / 1000;

	numNode += standardNumNode;
	pNodes->reserve(numNode);

	for(int i = 0; i < dev; ++i)
	{
		Pos seed = posGen();

		int sd = RandGen::unifi(
			static_cast<int>(standardNumNode * 0.5),
			static_cast<int>(standardNumNode * 1)//0.10
		); //��l���������ŕW���΍��𐶐�

		for(int j = 0; j < num / dev; ++j)
		{
			auto& nml = RandGen::nml; //���K��������������
			const auto pos = Pos(
				static_cast<int>(nml( seed.x, sd )),
				static_cast<int>(nml( seed.y, sd ))
			);

			pNodes->push_back( new Node(pos) );
		}
	}
}


//// �m�[�h���V�[�h�𒆐S�Ƃ����w�����z������
//void Simulator::AppnedNodes(int num)
//{
//	constexpr int dev = standardNumNode / 1000;
//
//	numNode += standardNumNode;
//	pNodes->reserve(numNode);
//
//	for( int i = 0; i < dev; ++i )
//	{
//		auto seed = posGen();
//		auto& exp = RandGen::exp; //�w����������������
//		auto& unifd = RandGen::unifd;
//
//		double lambda = RandGen::unifd(0.1, 0.5); //��l������lambda�𐶐�
//
//		for( int j = 0; j < num / dev; ++j )
//		{
//			int offsetX = 1000*exp(lambda) * cos(M_PI * unifd(0.01, 2));
//			int offsetY = 1000*exp(lambda) * sin(M_PI * unifd(0.01, 2));
//
//			const auto pos = Pos(
//				static_cast<int>(seed.x + offsetX),
//				static_cast<int>(seed.y + offsetY)
//			);
//
//			pNodes->push_back(new Node(pos));
//		}
//	}
//}



// -------------------- �����N���� --------------------


// void Simulator::GenerateLink() �Ŏg�p
auto generateLinkThread =
	[](std::vector<Node *>::iterator ipBegin1, std::vector<Node *>::iterator ipEnd1,
	   std::vector<Node *>::iterator ipBegin2, std::vector<Node *>::iterator ipEnd2, int idx)
	{
		extern Simulator simulator;

		for(auto ipNode1 = ipBegin1; ipNode1 != ipEnd1; ++ipNode1)
		{
			int cnt = 0;
			for(auto ipNode2 = ipBegin2; ipNode2 != ipEnd2; ++ipNode2)
			{
				if( *ipNode1 != *ipNode2 && Node::LinkExists(**ipNode1, **ipNode2))//, idx) )
				{
					(*ipNode1)->AddNeighbor(*ipNode2);
					++cnt;
				}
			}

			// �r���o�߂��X�V�imutex�t���ϐ��j
//			simulator.CompleteRate( idx, (double)std::distance(ipBegin1, ipNode1+1) / std::distance(ipBegin1, ipEnd1) );
			simulator.mtCompleteRate[idx]( (double)std::distance(ipBegin1, ipNode1+1) / std::distance(ipBegin1, ipEnd1) );
			simulator.mtNumLink += cnt;
		}
	};

void Simulator::GenerateLink()
{
	numLink = 0;

	const auto	ipBegin = std::begin(*pNodes),
				ipEnd   = std::end(*pNodes),
				ipMid   = std::begin(*pNodes) + std::distance(ipBegin, ipEnd) / 2;

	// 4�X���b�h����
	std::thread th1(generateLinkThread, ipBegin, ipMid, ipBegin, ipMid, 0);
	std::thread th2(generateLinkThread, ipBegin, ipMid, ipMid+1, ipEnd, 1);
	std::thread th3(generateLinkThread, ipMid+1, ipEnd, ipBegin, ipMid, 2);
	std::thread th4(generateLinkThread, ipMid+1, ipEnd, ipMid+1, ipEnd, 3);

	th1.detach();
	th2.detach();
	th3.detach();
	th4.detach();
}


void Simulator::ProcInput()
{
	Keyboard& kb = Keyboard::GetInstance();

	if( kb('F') && kb('I') == 1 ) { Initialize(); }
	if( kb('G') == 1 ) { DrawGraph(); }
	if( kb('T') == 1 ) { fGraphicalOut = !fGraphicalOut; }

	if( !kb(VK_SHIFT) )
	{
		const int moveDist = 20;
		Pos move(0, 0);
		if( kb(VK_LEFT)  ) { move += Pos( moveDist, 0); }
		if( kb(VK_RIGHT) ) { move += Pos(-moveDist, 0); }
		if( kb(VK_UP)    ) { move += Pos(0,  moveDist); }
		if( kb(VK_DOWN)  ) { move += Pos(0, -moveDist); }
		if( kb(VK_CONTROL) ) { move *= 3; }

		drawPos += move;
	}
	else if( kb(VK_UP)   ) { drawScale *= 1.02; }
	else if( kb(VK_DOWN) ) { drawScale *= 0.97; }
}


//========================================
//      �`�揈���i�R�[���o�b�N�����j
//========================================

void Simulator::DrawGraph()
{
	// �O���t�ݒ�
	gnuplot.SetLogScale(); //���ΐ�
//	gnuplot.SetLogScaleY(); //�Бΐ�
	gnuplot.SetXLabel( "degree" );
	gnuplot.SetYLabel( "num of vertex" );

	static std::vector<int> x( standardNumNode, 0 );
	for( int i = 0; i < standardNumNode; i++ ) { x[i] = i; }

	std::vector<int> degCount( standardNumNode, 0 );
	for( Node *pNode : *pNodes ) { ++degCount[pNode->Degree()]; }
//	for(Node *pNode : *pNodes) { ++degCount[ (int)pNode->Activity() ]; }

//	gnuplot.SetRange(0, 1000, 0, 10000);
	gnuplot.PlotXY( x, degCount );

}


void Simulator::MonitorOutput()
{
	std::stringstream buf;

	Monitor::mout() << FpsControl::GetInstance().GetInfo()
					<< Command::endline
					<< "[Simulation info]" << Command::endline
					<< "  node  : " << numNode << Command::endline
					<< "  link  : " << mtNumLink() << Command::endline
					<< Command::endline
					<< "[Completion]" << Command::endline;

	for(int i = 0; i < numThread; ++i)
	{
		Monitor::mout() << "  thread" << i+1 << " : " << MakeProgBar(mtCompleteRate[i]())
						<< "  (" << std::fixed << std::setprecision(3) << 100 * mtCompleteRate[i]() << "%)" << Command::endline;
	}

	Monitor::mout() << Command::endline
					<< "[Drawing info]" << Command::endline
					<< "  flag  : " << (fGraphicalOut ? "true" : "false") << Command::endline
					<< "  pos   : " << drawPos << Command::endline
					<< "  scale : " << drawScale * 100 << "%" << Command::endline;


	Monitor::mout(0) << buf.str() << Command::endline;
}