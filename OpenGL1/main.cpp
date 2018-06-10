#include "main.h"
#define ITERATION_SPAN 37

using namespace Core;
using namespace std;

int main(int argc, char **argv)
{
	floatColor colors[5];
	colors[0] = newFloatColor(0.0F, 0.0F, 1.0F);
	colors[1] = newFloatColor(0.3F, 0.2F, 0.7F);
	colors[2] = newFloatColor(1.0F, 0.5F, 0.0F);
	colors[3] = newFloatColor(0.6F, 0.5F, 0.5F);
	colors[4] = newFloatColor(0.0F, 0.0F, 1.0F);
	generateColorTable(ITERATION_SPAN, colors, 5);
	presentColorTable(table, ITERATION_SPAN);

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
	//cout << glewIsExtensionSupported("GLEW_ARB_gpu_shader_fp64") << endl;
	//cout << glewGetExtension("GLEW_ARB_gpu_shader_fp64") << endl;
	Init();
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(handleKey);
	glutMainLoop();
	glDeleteProgram(program);

	free(table);
	return 0;
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0);					//clear red
	glUseProgram(program);								//use the created program

	
	zoom(100);
	

	glDrawArrays(GL_QUADS, 0, 4);						//draw 4 vertices as a quad
	glutSwapBuffers();									//Swap image buffers
}

void resize (int w, int h)
{
	glViewport(0, 0, w, h);
	ratio = (long double)w / h;
}

void zoom(GLint maxiters)
{
	x = -0.6L;
	y = 0.0L;
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
		glUniform4d(loc, (GLdouble)vp.left, (GLdouble)vp.right, (GLdouble)vp.top, (GLdouble)vp.bottom);
	}

	loc = glGetUniformLocation(program, "colorTable");
	if (loc != -1)
	{
		glUniform3fv(loc, ITERATION_SPAN, table);    //2,9,6 =^= r,g,b
	}

	loc = glGetUniformLocation(program, "iterationSpan");
	if (loc != -1)
	{
		glUniform1i(loc, ITERATION_SPAN);
	}
}

void Init()
{
	glEnable(GL_DEPTH_TEST);

	//load and compile shaders
	Shader_Loader shaderLoader;
	program = shaderLoader.CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl", ITERATION_SPAN);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void menuCallback(int id) 
{
	cout << id << endl;
}

void generateColorTable(int iterationSpan, struct floatColor baseColors[], int numOfBaseColors)
{
	table = (GLfloat*)calloc(3 * iterationSpan, sizeof(GLfloat));		//...< ... .<.. .<.. ...
	int lastColorIndex = numOfBaseColors - 1;
	int numOfIntervs = numOfBaseColors - 1;
	if ((iterationSpan-1) % numOfIntervs == 0)			//numOfIntervs muss iterationSpan-1 teilen!!! 
	{
		int lengthOfInterv = (iterationSpan - 1) / numOfIntervs;
		/*
		0 iterations =^= colors[0]
		<iterationSpan> iteration =^= colors[end]
		.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//iterationSpan = 16 = 15 + 1
		,			,			,			,			,			,	//numberOfBaseColors = 6;	5 * interv(3)
		,					,					,					,	//numberOfBaseColors = 4;	3 * interv(5)
	
		.	.	.	.	.	.	.	.	.	.	.	.	.				//iterationSpan = 13
		.			.			.			.			.				//numOfBaseColors = 5; ninterv = nbasecolor
		*/
		{
			float rDiff, gDiff, bDiff, r, g, b, intervallAdvancement;		//All variables declared
			floatColor startBaseColor, endBaseColor;						//before the nested loop
			for (int i = 0; i < numOfIntervs; i++)							//to increase performance
			{												
				startBaseColor = baseColors[i];
				endBaseColor = baseColors[i + 1];
				rDiff = endBaseColor.x - startBaseColor.x;
				gDiff = endBaseColor.y - startBaseColor.y;
				bDiff = endBaseColor.z - startBaseColor.z;

				for (int j = 0; j < lengthOfInterv; j++)
				{
					intervallAdvancement = (float)j / lengthOfInterv;
					r = startBaseColor.x + rDiff * intervallAdvancement;
					g = startBaseColor.y + gDiff * intervallAdvancement;
					b = startBaseColor.z + bDiff * intervallAdvancement;
					table[3 * (i*lengthOfInterv + j)]     = r;
					table[3 * (i*lengthOfInterv + j) + 1] = g;
					table[3 * (i*lengthOfInterv + j) + 2] = b;
				}
			}
			table[3 * (iterationSpan - 1)]     = baseColors[numOfIntervs].x;
			table[3 * (iterationSpan - 1) + 1] = baseColors[numOfIntervs].y;
			table[3 * (iterationSpan - 1) + 2] = baseColors[numOfIntervs].z;
		}
	}
}

floatColor newFloatColor(float r, float g, float b)
{
	floatColor color;
	color.x = r;
	color.y = g;
	color.z = b;
	return color;
}

void presentColorTable(GLfloat *table, int iterationSpan)
{
	for (int i = 0; i<iterationSpan; i++)
	{
		cout << table[i * 3] << " " << table[i * 3 + 1] << " " << table[i * 3 + 2] << endl;
	}
}

void handleKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		//glDetachShader;
		
		glDeleteProgram(program);
		Init();
		renderScene();
		//glutDisplayFunc(renderScene);	//Not
		//glutReshapeFunc(resize);		//sure
		//glutKeyboardFunc(handleKey);	//on
		//glutMainLoop();				//these
		break;
	}
}