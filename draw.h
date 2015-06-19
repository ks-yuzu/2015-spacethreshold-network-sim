#pragma once

#include <functional>
#include <gl/freeglut.h>

#define M_PI 3.14159265358979323846

class Draw
{
	public:

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

	static void Circle(double cx, double cy, double r, bool fFill)
	{
		// ìhÇËÇ¬Ç‘Çµê›íË
		std::function<void(double, double, double, double, double, double)> drawLine =
			[](double cx, double cy, double rx1, double ry1, double rx2, double ry2){ Line(cx + rx1, cy + ry1, cx + rx2, cy + ry2); };
		std::function<void(double, double, double, double, double, double)> drawTriangle =
			[](double cx, double cy, double rx1, double ry1, double rx2, double ry2){ Triangle(cx + rx1, cy + ry1, cx + rx2, cy + ry2, cx, cy); };

		auto drawMethod = fFill ? drawTriangle : drawLine;

		// ï`âÊèàóù
		for(double theta1 = 0; theta1 < 360.0; theta1 += 1.0)
		{
			double theta2 = theta1 + 10.0;
			
			double thetaRad1 = theta1 / 180.0 * M_PI;
			double thetaRad2 = theta2 / 180.0 * M_PI;

			// â~é¸ç¿ïWåvéZ
			double rx1 = r * cos(thetaRad1);
			double ry1 = r * sin(thetaRad1);
			double rx2 = r * cos(thetaRad2);
			double ry2 = r * sin(thetaRad2);

			drawMethod(cx, cy, rx1, ry1, rx2, ry2);
		}

	}

};