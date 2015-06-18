//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <iostream>
#include <gl/glut.h>

#include "mtrand.h"
#include "simulator.h"

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//glRotated(25.0, 0.0, 1.0, 0.0);
	//glBegin(GL_POLYGON);
	//glColor3d(1.0, 0.0, 0.0); /* �� */
	//glVertex2d(-0.9, -0.9);
	//glColor3d(0.0, 1.0, 0.0); /* �� */
	//glVertex2d(0.9, -0.9);
	//glColor3d(0.0, 0.0, 1.0); /* �� */
	//glVertex2d(0.9, 0.9);
	//glColor3d(1.0, 1.0, 0.0); /* �� */
	//glVertex2d(-0.9, 0.9);
	//glEnd();

	glutSwapBuffers();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);			// �E�B���h�E�S�̂��r���[�|�[�g��
	glLoadIdentity();				// �ϊ��s��̏�����
	glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);
		//�X�N���[����̕\���̈���r���[�|�[�g�̑傫���ɔ�Ⴓ����

//	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
		//�X�N���[����̍��W�n���}�E�X�̍��W�n�Ɉ�v������
}


void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			printf("left");		break;
		case GLUT_MIDDLE_BUTTON:
			printf("middle");	break;
		case GLUT_RIGHT_BUTTON:
			printf("right");	break;
		default:				break;
	}

	printf(" button is ");

	switch (state)
	{
		case GLUT_UP:
			printf("up\n");		break;
		case GLUT_DOWN:
			printf("down\n");	break;
		default:				break;
	}

	printf(" at (%d, %d)\n", x, y);
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
//		case 'q':
//		case 'Q':
		case '\033':  // '\033' -> ESC
			exit(0);
	  default:
			break;
  }
}


Simulator simurator;


void InitializeGlut(int argc, char *argv[])
{
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("testWindow");
	glutDisplayFunc( [](){ simurator.Draw(); } );
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glClearColor(0.0, 0.0, 1.0, 1.0);
}


int main(int argc, char *argv[])
{
	InitializeGlut(argc, argv);

	int count[1024] = {0};

	auto expRand = RandGen::exp; //�w�����������̃����_

	for(int i = 0; i < 1000000; i++)
		count[(int)( expRand(1)*10 )]++;

	for(int i = 0; i < 40; i++)
	{
		for(int j = 0; j < count[i] / 1200; j++)  std::cout << "*";
		std::cout << std::endl;
	}

//	sim.AppnedNodes(1);

	glutMainLoop();

	int dummy;	std::cin >> dummy;

}