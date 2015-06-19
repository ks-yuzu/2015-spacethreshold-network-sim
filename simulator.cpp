#include <gl/glut.h>
#include <gl/freeglut_ext.h>
#include "define.h"
#include "simulator.h"
#include "mtrand.h"

//========================================
//              callback
//========================================
void Simulator::Draw()
{
	// vertex
	for each (Node *pNode in nodes)
	{
		Draw::Line( Pos(0, 0), pNode->GetPos() );

		for(auto it = pNode->Neighbors().cbegin(); it != pNode->Neighbors().cend(); it++)
		{
			it++;
			Draw::Line( (*it)->GetPos(), pNode->GetPos() );
		}
	}

	// node
	for each (Node *pNode in nodes) { pNode->Draw(); }
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
	while( true )
	{
		glutMainLoopEvent();	// イベント処理

		glutPostRedisplay();	// 描画イベント呼び出し
	}
}


void Simulator::Initialize()
{
	nodes.clear();
}


void Simulator::AppnedNodes(int num)
{
	for(int i = 0; i < num; i++)
		nodes.push_back( new Node() );
}


void Simulator::MakeVertex()
{
	auto it = nodes.begin();
	( *it )->AddNeighbor( *it + 1 );
}