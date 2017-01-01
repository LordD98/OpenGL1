#pragma once
//#pragma comment(lib, "glew/glew32.lib")

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader_Loader.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void renderScene(void);
void Init();	
void resize(int width, int height);
void zoom(GLint maxiters);
void menuCallback(int id);


GLuint program;
long double ratio = 1.5;
long double x, y;
struct ldRect 
{
	long double left;
	long double right;
	long double top;
	long double bottom;
} vp;