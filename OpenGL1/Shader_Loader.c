#include "Shader_Loader.h" 

//#include "Dependencies/GLUT/glut.h"

char* ReadShader(char* filename)
{	
	char* buffer = 0;
	long length;
	FILE * file = fopen(filename, "rb");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
		buffer = malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, file);
		}
		fclose(file);
	}
	else
	{
		printf( "Can't read file %s \n", filename );
		terminate();
	}
	return buffer;
}

GLuint CreateShader(GLenum shaderType, char*
	source, char* shaderName)
{

	int compile_result = 0;

	//GLuint shader = glCreateShader(shaderType);
	//const char *shader_code_ptr = source;	
	//const int shader_code_size = strlen(source);

	//glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	//glCompileShader(shader);
	//glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	////check for errors
	//if (compile_result == GL_FALSE)
	//{
	//	int info_log_length = 0;
	//	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
	//	char* shader_log(info_log_length);
	//	glGetShaderInfoLog(shader, info_log_length, NULL, shader_log);
	//	printf("ERROR compiling shader: %s\n%s\n", shaderName, shader_log);
	//	return 0;
	//}
	//return shader;
	return 0;
}

GLuint CreateProgram(char* vertexShaderFilename,
	char* fragmentShaderFilename, int iterationSpan)
{

	//read the shader files and save the code
	char* vertex_shader_code = ReadShader(vertexShaderFilename);
	char* fragment_shader_code = ReadShader(fragmentShaderFilename);
	
	//modify the shader code with regex to match the number of iterations contained in the table
	//also do other things with the shader code#
	//regex reg1("uniform vec3 colorTable\\[[0-9]*\\];");
	//fragment_shader_code = regex_replace(fragment_shader_code, reg1, "uniform vec3 colorTable[" + to_char*(iterationSpan) + "];");


	//GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	//GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");

	//int link_result = 0;
	////create the program handle, attatch the shaders and link it
	//GLuint program = glCreateProgram();
	//glAttachShader(program, vertex_shader);
	//glAttachShader(program, fragment_shader);

	//glLinkProgram(program);
	//glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	////check for link errors
	//if (link_result == GL_FALSE)
	//{

	//	//int info_log_length = 0;
	//	//glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
	//	//vector<char> program_log(info_log_length);
	//	//glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
	//	//printf("Shader Loader : LINK ERROR\n%s\n", &program_log[0]);
	//	return 0;
	//}
	//return program;
	return 0;
}