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


// 二分探索
std::vector<Node *>::iterator BinarySearch_Lower(const std::vector<Node *>::iterator& iFirst, const std::vector<Node *>::iterator& iLast, int reqiredActivity)
{
	// firstとlastの差で一致判定（STL的処理）
	auto diff = std::distance( iFirst, iLast );  // std::iterator_traits< std::vector<Node *>::iterator >::difference_type
	if ( diff == 0 ) return( iFirst );

	// 中央の位置を求める（STL的処理）
	std::vector<Node *>::iterator iMiddle = iFirst;
	std::advance( iMiddle, diff / 2 );

	if( reqiredActivity > (*iMiddle)->Activity() )
	{
		++iMiddle;
		return BinarySearch_Lower(iMiddle,  iLast, reqiredActivity);
	}
	else
	{
		return BinarySearch_Lower(iFirst, iMiddle, reqiredActivity);
	}
}


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
		for(auto ipNeighbor = pNode->LeastNeighbor(); ipNeighbor != ipEnd; ++ipNeighbor )
		{
			Draw::Line( pNode->GetPos(), (*ipNeighbor)->GetPos() );
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
//	for(Node *pNode : *pNodes) { ++degCount[ pNode->Degree() ]; }

//	SetRange(0, 1000, 0, 10000);
//	gnuplot.PlotXY(x, degCount);

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

	// 探索対象の先頭, 末尾イテレータを予め取得（速度重視）
	auto first = std::begin(*pNodes), last = std::end(*pNodes);

	for(auto iNode = first; iNode != last; ++iNode)
	{
		// 最小Activityの隣接ノードを二分探索
		(*iNode)->LeastNeighbor( BinarySearch_Lower(first, last, Node::threshold - (*iNode)->Activity()) );

		int deg = std::distance(iNode, (*iNode)->LeastNeighbor()) + 1;
		(*iNode)->Degree(deg);
		numLink += deg; // 合計
	}	
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