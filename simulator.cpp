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
	for each (Node node1 in nodes)
	{
		for each (Node *pNode2 in node1.Neighbors() )
		{
			Draw::Line( node1.GetPos(), pNode2->GetPos() );
		}
	}

	// node
	for each (Node node in nodes) { node.Draw(); }
}

void Simulator::WindowResize(int w, int h)
{
	glViewport(0, 0, w, h);													// ウィンドウ全体をビューポートに
	glLoadIdentity();														// 変換行列の初期化
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);	//スクリーン上の座標系をマウスの座標系に一致させる
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
		Monitor::mout(0) << "node : " << nodes.size() << Command::endline;

		Monitor::AllWindowFlip();
		fps.Update();
		fps.Wait();
	}
}


void Simulator::Initialize()
{
	numNode = 0;
	nodes.clear();

	AppnedNodes(standardNumNode);
	MakeEdge();

	// グラフ設定
	gnuplot.SetLogScale(); //両対数
	gnuplot.SetXLabel("degree");
	gnuplot.SetYLabel("num of vertex");

	std::vector<int> x(standardNumNode);
	std::vector<int> degCount(standardNumNode);

//	for(int i = 0; i < standardNumNode; i++) { x[i] = i; }

//	for(auto it = nodes.begin(); it != nodes.end(); it++)
//	{
//		degCount[ it->Neighbors().size() ]++;
//	}

//	SetRange(0, 1000, 0, 10000);
//	gnuplot.PlotXY(x, degCount);


	// ここきれいに nodexポインタに 移動 空間 キー処理（透過度）　マルチスレッド
}


void Simulator::AppnedNodes(int num = standardNumNode)
{
	numNode += standardNumNode;
	for(int i = 0; i < num; i++) { nodes.push_back( Node() ); }
}


void Simulator::MakeEdge()
{
	for(auto it1 = nodes.begin(); it1 != nodes.end(); it1++)
	{
		for(auto it2 = nodes.begin(); it2 != nodes.end(); it2++)
		{
			if( EdgeExists(*it1, *it2) ) { it1->AddNeighbor( *it2 ); }
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

//	if( kb('I') == 1)
		{ Initialize(); }
}