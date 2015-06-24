#pragma once

#include <functional>
#include <gl/freeglut.h>

#define M_PI 3.14159265358979323846

class Draw
{
	public:

	static void Line(Pos p1, Pos p2)
	{
		Line(p1.x, p1.y, p2.x, p2.y);
	}

	static void Line(double x1, double y1, double x2, double y2)
	{
		glBegin(GL_LINES);
		  glVertex2d(x1, y1);
		  glVertex2d(x2, y2);
		glEnd();
	}

	static void Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
	{
		glBegin(GL_TRIANGLES);
		  glVertex2d(x1, y1);
		  glVertex2d(x2, y2);
		  glVertex2d(x3, y3);
		glEnd();
	}

	inline static void Circle(Pos center, int r, bool fFill)
	{
		Circle(center.x, center.y, r, fFill);
	}

	static void Circle(int cx, int cy, int r, bool fFill)
	{
		//// ìhÇËÇ¬Ç‘Çµê›íË
		//std::function<void(double, double, double, double, double, double)> drawLine =
		//	[](double cx, double cy, double rx1, double ry1, double rx2, double ry2){ Line(cx + rx1, cy + ry1, cx + rx2, cy + ry2); };
		//std::function<void(double, double, double, double, double, double)> drawTriangle =
		//	[](double cx, double cy, double rx1, double ry1, double rx2, double ry2){ Triangle(cx + rx1, cy + ry1, cx + rx2, cy + ry2, cx, cy); };

		//auto drawMethod = fFill ? drawTriangle : drawLine;

		// ï`âÊèàóù
		const int numVertex = 30;
		float* vertexes = new float[2*numVertex];

		for(int i = 0; i < numVertex; ++i)
		{
			float thetaRad = static_cast<float>(2 * M_PI * i / numVertex);

			vertexes[2*i]   = cx + r * std::cos(thetaRad);
			vertexes[2*i+1] = cy + r * std::sin(thetaRad);
		}
		
		glVertexPointer(2, GL_FLOAT, 0, vertexes);

		glEnable(GLUT_MULTISAMPLE);
		  glMatrixMode(GL_MODELVIEW);
		  glEnableClientState(GL_VERTEX_ARRAY);
			glDrawArrays(GL_TRIANGLE_FAN, 0, numVertex);
		  glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GLUT_MULTISAMPLE);
	}

};


