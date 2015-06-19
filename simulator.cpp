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
	for each (Node *pNode in nodes) { pNode->Draw(); }

	// neighbors
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
	while( true )
	{
		glutMainLoopEvent();	// �C�x���g����

		glutPostRedisplay();	// �`��C�x���g�Ăяo��
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
