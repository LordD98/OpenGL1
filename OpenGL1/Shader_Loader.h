#pragma once

//#define string char*

#include <iostream>
#include "main.h"
//#include <vector>

GLuint CreateProgram(char* vertexShaderFilename, char* fragmentShaderFilename, int iterationSpan);
GLuint CreateShader(GLenum shaderType, char* source, char* shaderName);
char* ReadShader(char* filename);
