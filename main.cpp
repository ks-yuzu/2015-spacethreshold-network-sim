#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <iostream>
#include <gl/glut.h>
#include <gl/GL.h>

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
//    -> WindowsAPIに変更

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

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
int main(int argc, char *argv[])
{
//	int argc = 1;  LPTSTR argv = "dummy";
	InitializeGlut(argc, argv);

//	Log::lout().Init();
//	Log::lout() << "Log Window" << Command::endline << Command::endline; //ログウィンドウの設定とタイトル


	simulator.Initialize();

	simulator.MainLoop();

	return 0;
}
