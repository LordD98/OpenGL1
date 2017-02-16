#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\GLUT\glut.h"

#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>	
//#include <string.h>

int main(int argc, char ** argv);

void renderScene(void);
void Init();	
void resize(int width, int height);
void handleKey(unsigned char key, int x, int y);
void zoom(GLint maxiters);
void menuCallback(int id);
void generateColorTable(int iterationSpan, struct floatColor *baseColors, int numOfBaseColors);
void presentColorTable(GLfloat *table, int iterationSpan);
char* ReadShader(char* filename);
GLuint CreateShader(GLenum shaderType, char* source, char* shaderName);
GLuint CreateProgram(char* vertexShaderFilename, char* fragmentShaderFilename, int iterationSpan);
struct floatColor newFloatColor(float r, float g, float b);

GLfloat *table;
int iterationSpan;
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