#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader_Loader.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
void renderScene(void);
void Init();
void resize(int width, int height);
void zoom(int maxiters);


GLuint program;
float ratio;
float vpRect[4];