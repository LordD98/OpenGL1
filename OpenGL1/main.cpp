#include "main.h"

using namespace Core;
using namespace std;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(720, 480);
	glutCreateWindow("OpenGL Mandelbrot");
	
	//glutCreateMenu(menuCallback);
	//glutAddMenuEntry("File", 0);
	//glutAddMenuEntry("Tools", 1);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	glewInit();
	Init();
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
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

	zoom(100);
	
	//draw 4 vertices as a quad
	glDrawArrays(GL_QUADS, 0, 4);

	glutSwapBuffers();
}

void resize (int w, int h)
{
	glViewport(0, 0, w, h);
	ratio = (long double)w / h;
}

void zoom(GLint maxiters)
{
	cout << "zoomstart!" << endl;
	x = -0.6F;
	y = 0.0F;
	long double dy = 2.8L;
	long double dx = dy * ratio;

	vp.left= x - dx / 2;
	vp.right = x + dx / 2;
	vp.top = y + dy / 2;
	vp.bottom = y - dy / 2;


	GLint loc = glGetUniformLocation(program, "maxiters");
	if (loc != -1)
	{
		glUniform1i(loc, maxiters);
	}
	
	loc = glGetUniformLocation(program, "rect");
	if (loc != -1)
	{
		glUniform4f(loc, (GLfloat)vp.left, (GLfloat)vp.right, (GLfloat)vp.top, (GLfloat)vp.bottom);
	}
}

void Init()
{
	glEnable(GL_DEPTH_TEST);

	//load and compile shaders
	Shader_Loader shaderLoader;
	program = shaderLoader.CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void menuCallback(int id) 
{
	cout << id << endl;
}