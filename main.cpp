//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <iostream>
#include <gl/glut.h>

#include "simulator.h"
#include "input.h"
#include "mtrand.h"


Simulator simulator;
Input input;

void InitializeGlut(int argc, char *argv[])
{
	auto dispCallback = 
		[](){
			glClear(GL_COLOR_BUFFER_BIT);
			simulator.Draw();
			glutSwapBuffers();
		};
	auto dummyCallback = [](){};

	auto reshapeCallback = [](int w, int h){ simulator.WindowResize(w, h); };
	auto mouseCallback = [](int button, int state, int x, int y){ input.ProcMouseEvent(button, state, x, y); };
	auto kbCallback = [](unsigned char key, int x, int y){ input.ProcKbEvent(key, x, y); };

	glutInitWindowPosition(windowPos.x, windowPos.y);
	glutInitWindowSize(windowSize.x, windowSize.y);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(title.c_str());
	glutDisplayFunc( dispCallback );
	glutReshapeFunc( reshapeCallback );
	glutMouseFunc( mouseCallback );
	glutKeyboardFunc( kbCallback );
	glClearColor(0, 0, 0, 1.0);
}


int main(int argc, char *argv[])
{
	InitializeGlut(argc, argv);

	simulator.Initialize();
	simulator.AppnedNodes(10);

	simulator.MakeVertex();

	simulator.MainLoop();
}



/*

	int count[1024] = {0};

	auto expRand = RandGen::exp; //éwêîóêêîê∂ê¨ÇÃÉâÉÄÉ_

	for(int i = 0; i < 1000000; i++)
		count[(int)( expRand(1)*10 )]++;

	for(int i = 0; i < 40; i++)
	{
		for(int j = 0; j < count[i] / 1200; j++)  std::cout << "*";
		std::cout << std::endl;
	}

*/
