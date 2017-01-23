#pragma once
//#pragma comment(lib, "glew/glew32.lib")

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader_Loader.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv);

void renderScene(void);
void Init();	
void resize(int width, int height);
void handleKey(unsigned char key, int x, int y);
void zoom(GLint maxiters);
void menuCallback(int id);
void generateColorTable(int iterationSpan, struct floatColor baseColors[], int numOfBaseColors);
void presentColorTable(GLfloat *table, int iterationSpan);
struct floatColor newFloatColor(float r, float g, float b);

GLfloat *table;
GLuint program;
long double ratio = 1.5L;
long double x, y;

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