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

//========================================
//              callback
//========================================
void Simulator::Draw()
{
	// vertex
	glColor4d(0.4, 0.9, 0.4, 0.2);
	for(Node *pNode1 : *pNodes)
	{
		for (Node *pNode2 : pNode1->Neighbors() )
		{
			Draw::Line( pNode1->GetPos(), pNode2->GetPos() );
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
//	Log::cout.Init();
//	Log::cout << "Log Window" << Command::endline << Command::endline; //ログウィンドウの設定とタイトル


	while( fps.SetStartTime(), kb.Update() )
	{
		// イベント処理
		glutMainLoopEvent();

		// キー入力処理
		ProcInput();


		// 描画イベント呼び出し
		glutPostRedisplay();	

		// ループ末処理
		Monitor::mout(0) << "node : " << pNodes->size() << Command::endline;

		Monitor::AllWindowFlip();
		fps.Update();
		fps.Wait();
	}
}

void Simulator::Initialize()
{
	numNode = 0;

	if( pNodes != nullptr )
	{
		auto deleteNodeVector =
			[](std::vector<Node *> *pNodes)
			{
				for(Node *p : *pNodes){ delete p; }
				delete pNodes;
			};

		std::thread delNodes(deleteNodeVector, pNodes);
		delNodes.detach();
	}

	pNodes = new std::vector<Node *>;
	AppnedNodes(standardNumNode);
	MakeEdge();

	// グラフ設定
	gnuplot.SetLogScale(); //両対数
	gnuplot.SetXLabel("degree");
	gnuplot.SetYLabel("num of vertex");

	std::vector<int> x(standardNumNode);
	std::vector<int> degCount(standardNumNode);

	for(int i = 0; i < standardNumNode; i++) { x[i] = i; }
	for(Node *pNode : *pNodes) { ++degCount[ pNode->Neighbors().size() ]; }

//	SetRange(0, 1000, 0, 10000);
	gnuplot.PlotXY(x, degCount);


	// ここきれいに+別プロセス 移動 空間 キー処理（透過度）　マルチスレッド
}


void Simulator::AppnedNodes(int num)
{
	numNode += standardNumNode;
	pNodes->reserve( numNode );

	for(int i = 0; i < num; i++) { pNodes->push_back( new Node() ); }

	std::sort(pNodes->begin(), pNodes->end(), [](Node *n1, Node *n2){return n1->Activity() < n2->Activity();});
}


void Simulator::MakeEdge()
{
	int count = 0;
	for(Node *n1 : *pNodes)
	{
		for(Node *n2 : *pNodes)
		{
			if( EdgeExists(*n1, *n2) ) { n1->AddNeighbor( n2 ); }
			count++;
		}
	}
}


inline bool Simulator::EdgeExists(const Node& n1, const Node& n2) const
{
	// kuukann　モデル
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