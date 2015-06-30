#include <algorithm>
#include <numeric>
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
//		glutPostRedisplay();	

		// ループ末処理
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

	// 別スレッドで解放
	if( pNodes != nullptr ) { Deleter<Node> deleter(pNodes); }

	pNodes = new std::vector<Node *>;
	AppnedNodes(standardNumNode);
	GenerateLink();

	// グラフ設定
	gnuplot.SetLogScale(); //両対数
	gnuplot.SetXLabel("degree");
	gnuplot.SetYLabel("num of vertex");

	std::vector<int> x(standardNumNode);
	std::vector<int> degCount(standardNumNode);

	for(int i = 0; i < standardNumNode; i++) { x[i] = i; }
	for(Node *pNode : *pNodes) { ++degCount[ pNode->Degree() ]; }
//	for(Node *pNode : *pNodes) { ++degCount[ (int)pNode->Activity() ]; }

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

// 下の void Simulator::GenerateLink() から使用
auto GenerateLinkThread =
	[](std::vector<Node *>::iterator ipBegin1, std::vector<Node *>::iterator ipEnd1,
	   std::vector<Node *>::iterator ipBegin2, std::vector<Node *>::iterator ipEnd2 )
	{
		for(auto ipNode1 = ipBegin1; ipNode1 != ipEnd1; ++ipNode1)
		{
			for(auto ipNode2 = ipBegin2; ipNode2 != ipEnd2; ++ipNode2)
			{
				if( Node::LinkExists(**ipNode1, **ipNode2) )
				{
					(*ipNode1)->AddNeighbor(*ipNode2);
				}
			}
		}
	};

void Simulator::GenerateLink()
{
	numLink = 0;

	const auto
		ipBegin = std::begin(*pNodes),
		ipEnd = std::end(*pNodes),
		ipMid = std::begin(*pNodes) + std::distance(ipBegin, ipEnd) / 2;

	// 4スレッド並列
	std::thread th1(GenerateLinkThread, ipBegin, ipMid, ipBegin, ipMid);
	std::thread th2(GenerateLinkThread, ipBegin, ipMid, ipMid+1, ipEnd);
	std::thread th3(GenerateLinkThread, ipMid+1, ipEnd, ipBegin, ipMid);
	std::thread th4(GenerateLinkThread, ipMid+1, ipEnd, ipMid+1, ipEnd);

	th1.join();
	th2.join();
	th3.join();
	th4.join();

	numLink = std::accumulate(std::begin(*pNodes), std::end(*pNodes), 0, [](int sum, const Node *pNode2){ return sum + pNode2->Degree(); });
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