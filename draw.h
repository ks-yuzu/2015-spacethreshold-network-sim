#pragma once

#include <functional>
#include <gl/freeglut.h>
#include "define.h"

class Draw
{
	public:

	inline static void Line(Pos p1, Pos p2)
	{
		Line(p1.x, p1.y, p2.x, p2.y);
	}

	inline static void Line(double x1, double y1, double x2, double y2)
	{
		glBegin(GL_LINES);
		  glVertex2d(x1, y1);
		  glVertex2d(x2, y2);
		glEnd();
	}

	inline static void Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
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
		// •`‰æˆ—
		const int numVertex = 6;
		double vertexes[2*numVertex]; // ‘¬“xd‹‚Å¶”z—ñ

		for(int i = 0; i < numVertex; ++i)
		{
			double theta = 2 * M_PI * i / numVertex;
			vertexes[2*i]   = cx + r * std::cos(theta);
			vertexes[2*i+1] = cy + r * std::sin(theta);
		}
		
		glVertexPointer(2, GL_DOUBLE, 0, vertexes);

		glEnable(GLUT_MULTISAMPLE);
		  glMatrixMode(GL_MODELVIEW);
		  glEnableClientState(GL_VERTEX_ARRAY);
			glDrawArrays(GL_TRIANGLE_FAN, 0, numVertex);
		  glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GLUT_MULTISAMPLE);
	}

	static void Lines(const int *pVertexes, const int numVertex, const int dim)
	{
		glVertexPointer(2, GL_INT, 0, pVertexes);

		glEnable(GLUT_MULTISAMPLE);
		glMatrixMode(GL_MODELVIEW);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_LINES, 0, numVertex);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GLUT_MULTISAMPLE);
	}
};


