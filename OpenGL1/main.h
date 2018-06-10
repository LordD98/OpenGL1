#pragma once
//#pragma comment(lib, "glew/glew32.lib")

#define PIXEL_STRAFE 10			// defines the number of pixels that strafing (left, right, up & down) moves the rendering
#define PIXEL_STRAFE_LD 10.0L	// -- " -- (but as a long double)
#define MAXITERS 100			// defines the maximum number of iterations of the shader main loop
#define ITERATION_SPAN 37		// defines the number of iterations that span over one baseColor

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader_Loader.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv);

void Init(void);												
void renderScene(void);
void setParams(long double rPos, long double iPos, long double ySpan, GLint maxiters);
void inspectPoint(long double rPos, long double iPos);

void handleResize(int width, int height);					// GLUT functions
void handleKey(unsigned char key, int x, int y);			// and callbacks
void handleSpecialKey(int key, int x, int y);				//
void handleMouse(int button, int state, int x, int y);		// 
void handleMouseMotion(int x, int y);						// 
void handleMousePassiveMotion(int x, int y);				// 
void menuCallback(int id);									// 

void generateColorTable(int iterationSpan, struct floatColor baseColors[], int numOfBaseColors);
void presentColorTable(GLfloat *table, int iterationSpan);
struct floatColor newFloatColor(float r, float g, float b);


GLuint program;
GLfloat *table;						// stores the colors of the rendering
long double ratio = 1.5L;			// proportion of the viewport (and of the rendering) (X/Y)
long double X = -0.6L;				// Center point of the complex plane (real part)
long double Y = 0.0L;				//	 -		-	"	-	  -		(imaginary part)
int WIDTH, HEIGHT;					//dimensions of the viewport


long double oldX, oldY;				// Temporarily save the center point (when lmb is pressed)

long double spanY = 2.8L;			// Height of the complex plane
long double spanX = spanY * ratio;	// Width  of the complex plane


GLint mouseDownPositionX, mouseDownPositionY;	//position where lmb was pressed
GLint mousex, mousey;			//cursor position
bool lmbPressed = false;		//left mouse button pressed


struct ldRect 
{
	long double left;
	long double right;
	long double top;
	long double bottom;
} vp;

struct floatColor
{
	float x, y, z;
};