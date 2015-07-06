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
	glPushMatrix();
	glTranslated(drawPos.x, drawPos.y, 0);
	glScaled(drawScale, drawScale, drawScale);

	// --- 描画処理開始 ---

	// vertex
	glColor4d(0.4, 0.9, 0.4, 0.2);

	for(Node *pNode : *pNodes)
	{
		for(auto pNeighbor : pNode->Neighbors())
		{
			//std::lock_guard<std::mutex> lock(*(pNeighbor->pNeighborsMtx));
			Draw::Line( pNode->GetPos(), pNeighbor->GetPos() );
		}
	}

	// node
	for each (Node *pNode in *pNodes) { pNode->Draw(); }
	
	// --- 描画処理ここまで ---
	glPopMatrix();
}

void Simulator::WindowResize(int w, int h)
{
	glViewport(0, 0, w, h);													// ウィンドウ全体をビューポートに
	glLoadIdentity();														// 変換行列の初期化
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);	// スクリーン上の座標系をマウスの座標系に一致させる
}


//========================================
//              operator
//========================================

void Simulator::MainLoop()
{
	Keyboard& kb = Keyboard::GetInstance();
	FpsControl& fps = FpsControl::GetInstance();

	while( fps.SetStartTime(), kb.Update() )
	{
		// イベント処理
		glutMainLoopEvent();

		// キー入力処理
		ProcInput();

		// 描画イベント呼び出し
		glutPostRedisplay();

		// ループ末処理
		MonitorOutput();
		Monitor::AllWindowFlip();
		fps.Update();
		fps.Wait();
	}
}


void Simulator::Initialize()
{
	// 別スレッドで解放
	if( pNodes != nullptr ) { Deleter<Node> deleter(pNodes); }

	numNode = 0;

	pNodes = new std::vector<Node *>;
	AppnedNodes(standardNumNode);
	GenerateLink();
}


//void Simulator::AppnedNodes(int num)
//{
//	numNode += standardNumNode;
//	pNodes->reserve( numNode );
//
//	for(int i = 0; i < num; i++) { pNodes->push_back( new Node() ); }
//
////	std::sort(std::begin(*pNodes), std::end(*pNodes), [](Node *n1, Node *n2){ return n1->Activity() < n2->Activity(); });
//}



void Simulator::AppnedNodes(int num)
{
	constexpr int dev = 10;

	numNode += standardNumNode;
	pNodes->reserve(numNode);

	for(int i = 0; i < dev; ++i)
	{
		Node *pCenterNode = new Node();
		pNodes->push_back(pCenterNode);

		auto& nml = RandGen::nml;

		//for( int j = 0; j < num / dev - 1; ++j )
		//{
		//	pNodes->push_back(new Node(Pos(nml(pCenterNode->GetPos().x, nml(20000,100)), nml(pCenterNode->GetPos().y, nml(20000, 100)))));
		//}

		for(int j = 0; j < num / dev /2 -1; ++j)
		{
			pNodes->push_back(new Node( Pos(nml(pCenterNode->GetPos().x, 10000), nml(pCenterNode->GetPos().y, 10000)) ));
			pNodes->push_back(new Node( Pos(nml(pCenterNode->GetPos().x, 20000), nml(pCenterNode->GetPos().y, 20000)) ));
//			pNodes->push_back(new Node(Pos(nml(pCenterNode->GetPos().x, nml(20000,100)), nml(pCenterNode->GetPos().y, nml(20000, 100)))));
		}
		pNodes->push_back(new Node(Pos(nml(pCenterNode->GetPos().x, 10000), nml(pCenterNode->GetPos().y, 10000)))); // 数合わせ
	}

	//	std::sort(std::begin(*pNodes), std::end(*pNodes), [](Node *n1, Node *n2){ return n1->Activity() < n2->Activity(); });
}





// void Simulator::GenerateLink() で使用
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

			// mutex付きオペレータで途中経過を更新
			simulator.CompleteRate( idx, (double)std::distance(ipBegin1, ipNode1+1) / std::distance(ipBegin1, ipEnd1) );
			simulator.mtNumLink += cnt;
		}
	};

void Simulator::GenerateLink()
{
	numLink = 0;

	const auto	ipBegin = std::begin(*pNodes),
				ipEnd   = std::end(*pNodes),
				ipMid   = std::begin(*pNodes) + std::distance(ipBegin, ipEnd) / 2;

	// 4スレッド並列
	std::thread th1(generateLinkThread, ipBegin, ipMid, ipBegin, ipMid, 0);
	std::thread th2(generateLinkThread, ipBegin, ipMid, ipMid+1, ipEnd, 1);
	std::thread th3(generateLinkThread, ipMid+1, ipEnd, ipBegin, ipMid, 2);
	std::thread th4(generateLinkThread, ipMid+1, ipEnd, ipMid+1, ipEnd, 3);

	th1.detach();
	th2.detach();
	th3.detach();
	th4.detach();
}



void Simulator::DrawGraph()
{
	// グラフ設定
	gnuplot.SetLogScale(); //両対数
	gnuplot.SetXLabel("degree");
	gnuplot.SetYLabel("num of vertex");

	static std::vector<int> x(standardNumNode, 0);
	for(int i = 0; i < standardNumNode; i++) { x[i] = i; }

	std::vector<int> degCount(standardNumNode, 0);
	for(Node *pNode : *pNodes) { ++degCount[ pNode->Degree() ]; }
//	for(Node *pNode : *pNodes) { ++degCount[ (int)pNode->Activity() ]; }

//	gnuplot.SetRange(0, 1000, 0, 10000);
	gnuplot.PlotXY(x, degCount);

}



void Simulator::ProcInput()
{
	Keyboard& kb = Keyboard::GetInstance();

	if( kb('I') == 1 ) { Initialize(); }
	if( kb('G') == 1 ) { DrawGraph(); }

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



void Simulator::MonitorOutput()
{
	Monitor::mout(0) << FpsControl::GetInstance().GetInfo();

	Monitor::mout(0) << Command::endline;
	Monitor::mout(0) << "[Simulation info]" << Command::endline;
	Monitor::mout(0) << "  node  : " << numNode << Command::endline;
	Monitor::mout(0) << "  link  : " << mtNumLink() << Command::endline;

	Monitor::mout(0) << Command::endline;
	Monitor::mout(0) << "[Completion level of generating link]" << Command::endline;
	for(int i = 0; i < numThread; ++i)
	{
		std::stringstream buf;
		buf << "  thread" << i+1 << " : " << MakeProgBar(CompleteRate(i)) << "  (" << std::fixed << std::setprecision(2) << 100 * CompleteRate(i) << "%)";
		Monitor::mout(0) << buf.str() << Command::endline;
	}

	Monitor::mout(0) << Command::endline;
	Monitor::mout(0) << "[Drawing info]" << Command::endline;
	Monitor::mout(0) << "  pos   : " << drawPos << Command::endline;
	Monitor::mout(0) << "  scale : " << drawScale << Command::endline;
}