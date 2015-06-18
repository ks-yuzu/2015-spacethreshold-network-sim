//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <iostream>
#include <gl/glut.h>

#include "simulator.h"
#include "input.h"
#include "mtrand.h"


Simulator simurator;
Input input;

void Resize(int, int);

void InitializeGlut(int argc, char *argv[])
{
	auto dispCallback = 
		[](){
			glClear(GL_COLOR_BUFFER_BIT);
			simurator.Draw();
			glutSwapBuffers();
		};

	auto mouseCallback = [](int button, int state, int x, int y){ input.ProcMouseEvent(button, state, x, y); };
	auto kbCallback = [](unsigned char key, int x, int y){ input.ProcKbEvent(key, x, y); };

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(title.c_str());
	glutDisplayFunc( dispCallback );
	glutReshapeFunc(Resize);
	glutMouseFunc( mouseCallback );
	glutKeyboardFunc( kbCallback );
	glClearColor(0.0, 0.0, 1.0, 1.0);
}


int main(int argc, char *argv[])
{
	InitializeGlut(argc, argv);

	int count[1024] = {0};

	auto expRand = RandGen::exp; //指数乱数生成のラムダ

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


void Resize(int w, int h)
{
	glViewport(0, 0, w, h);			// ウィンドウ全体をビューポートに
	glLoadIdentity();				// 変換行列の初期化
	glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);
		//スクリーン上の表示領域をビューポートの大きさに比例させる

//	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
		//スクリーン上の座標系をマウスの座標系に一致させる
}
