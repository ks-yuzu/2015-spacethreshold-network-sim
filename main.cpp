#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <iostream>
#include <gl/glut.h>

#include "simulator.h"
#include "input/mouse.h"

Simulator simulator;
Mouse mouse;

void InitializeGlut(int argc, char *argv[])
{
	auto dispCallback = 
		[]{
			glClear(GL_COLOR_BUFFER_BIT);
			simulator.Draw();
			glutSwapBuffers();
		};
	auto dummyCallback = []{};

	auto reshapeCallback = [](int w, int h){ simulator.WindowResize(w, h); };
	auto mouseCallback = [](int button, int state, int x, int y){ mouse.ProcMouseEvent(button, state, x, y); };
//	auto kbCallback = [](unsigned char key, int x, int y){ input.ProcKbEvent(key, x, y); };

	glutInitWindowPosition(windowPos.x, windowPos.y);
	glutInitWindowSize(windowSize.x, windowSize.y);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(title.c_str());
//	glutDisplayFunc( dummyCallback );
	glutDisplayFunc( dispCallback );
	glutReshapeFunc( reshapeCallback );
	glutMouseFunc( mouseCallback );
//	glutKeyboardFunc( kbCallback );
	glClearColor(0, 0, 0, 1.0);

	//アルファブレンドの設定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}


int main(int argc, char *argv[])
{
	InitializeGlut(argc, argv);

	simulator.Initialize();

	simulator.MainLoop();

	return 0;
}
