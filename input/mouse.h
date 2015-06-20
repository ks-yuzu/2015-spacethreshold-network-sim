#pragma once

#include <iostream>
#include <gl/glut.h>

class Mouse
{
	public:
		void ProcMouseEvent(int button, int state, int x, int y);
//		void ProcKbEvent(unsigned char key, int x, int y);
};


inline void Mouse::ProcMouseEvent(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			std::cout << "left";	break;
		case GLUT_MIDDLE_BUTTON:
			std::cout << "middle";	break;
		case GLUT_RIGHT_BUTTON:
			std::cout << "right";	break;
		default:					break;
	}

	std::cout << " button is ";

	switch (state)
	{
		case GLUT_UP:
			std::cout << "up\n";	break;
		case GLUT_DOWN:
			std::cout << "down\n";	break;
		default:					break;
	}

	std::cout << " at (" << x << ", " << y << ")\n" << std::endl;
}

//inline void Input::ProcKbEvent(unsigned char key, int x, int y)
//{
//	switch (key) {
////		case 'q':
////		case 'Q':
//		case '\033':  // '\033' -> ESC
//			exit(0);
//	  default:
//			break;
//  }
//}
