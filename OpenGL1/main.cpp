#include "main.h"

using namespace Core;

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1080, 720);
	glutCreateWindow("OpenGL First Window");

	glewInit();

	Init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	zoom(100);
	resize(1080, 720);
	glutMainLoop();
	glDeleteProgram(program);
	return 0;
}

void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0);//clear red


	//use the created program
	glUseProgram(program);

	//draw 3 vertices as triangles
	glDrawArrays(GL_QUADS, 0, 4);

	glutSwapBuffers();
}

void Init()
{

	glEnable(GL_DEPTH_TEST);

	//load and compile shaders
	Core::Shader_Loader shaderLoader;
	program = shaderLoader.CreateProgram("Vertex_Shader.glsl",
		"Fragment_Shader.glsl");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void resize (int w, int h)
{
	glViewport(0, 0, w, h);
	GLint *vp = new GLint[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	int width = vp[2], height = vp[3];
	std::cout << " " << width << " " << height << std::endl;
	ratio = (float)width / height;

	GLint loc = glGetUniformLocation(program, "ratio");
	if (loc != -1)
	{
		glUniform1f(loc, ratio);
	}
}

void zoom(GLint maxiters)
{
	GLint loc = glGetUniformLocation(program, "maxiters");
	if (loc != -1)
	{
		glUniform1f(loc, maxiters);
	}
	vpRect[0] = 1.4F;
	vpRect[1] = -1.4F;
	vpRect[2] = -2.2F;
	vpRect[3] = 1.0F;

	loc = glGetUniformLocation(program, "top");
	if (loc != -1)
	{
		glUniform1f(loc, 1.4F);
	}

	loc = glGetUniformLocation(program, "bottom");
	if (loc != -1)
	{
		glUniform1f(loc, -1.4F);
	}

	loc = glGetUniformLocation(program, "left");
	if (loc != -1)
	{
		glUniform1f(loc, -2.2F);
	}

	loc = glGetUniformLocation(program, "right");
	if (loc != -1)
	{
		glUniform1f(loc, 1.0F); //right
	}

	std::cout << "test" << std::endl;
}