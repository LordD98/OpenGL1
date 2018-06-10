// change of real part = (dx = change of mouse in pixels)/WIDTH*spanX
// pixels = (dr = change of real part)/spanX * WIDTH


#include "main.h"

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
	//presentColorTable(table, ITERATION_SPAN);

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
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(handleKey);
	glutSpecialFunc(handleSpecialKey);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMouseMotion);
	glutPassiveMotionFunc(handleMousePassiveMotion);

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

	setParams(X, Y, spanY, MAXITERS);					// starts with -0.6, 0.0, 2.8, 100

	glDrawArrays(GL_QUADS, 0, 4);						//draw 4 vertices as a quad
	glutSwapBuffers();									//Swap image buffers
}

void handleMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseDownPositionX = x;
			mouseDownPositionY = y;
			oldX = X;
			oldY = Y;
			lmbPressed = true;
		}
		else if (state == GLUT_UP)
		{
			lmbPressed = false;
		}
	}
}

void handleMouseMotion(int x, int y)
{
	mousex = x;
	mousey = y;
	//cout << "Motion:" << x << ", " << y << endl;
	if (lmbPressed)
	{
		GLint dx = -(x - mouseDownPositionX);	// X has to be inverted here
		GLint dy =   y - mouseDownPositionY;
		X = oldX + static_cast<long double>(dx)/static_cast<long double>(WIDTH) * spanX;
		Y = oldY + static_cast<long double>(dy) / static_cast<long double>(HEIGHT) * spanY;
		//cout << "Moved by " << dx << ", " << dy << endl;
		glutPostRedisplay();
	}
}

void handleMousePassiveMotion(int x, int y)
{
	mousex = x;
	mousey = y;
	
	//cout << "Passive motion:" << x << ", " << y << endl;
}

void handleResize (int w, int h)
{
	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, w, h);
	ratio = (long double)w / h;
}

void setParams(long double rPos, long double iPos, long double ySpan, GLint maxiters)
{
	X = rPos;
	Y = iPos;
	spanY = ySpan;
	spanX = spanY * ratio;

	vp.left= X - spanX / 2;
	vp.right = X + spanX / 2;
	vp.top = Y + spanY / 2;
	vp.bottom = Y - spanY / 2;


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
	glutPostRedisplay();	// UNNECESSARY (just to test performance)
	//cout << "Zoom" << endl;
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
	case 'c':
		setParams(-0.6L, 0.0L, 2.8L, MAXITERS);
		glutPostRedisplay();
		glutWarpPointer(WIDTH/2, HEIGHT/2);
		break;
	case 'f':
		glutFullScreenToggle();
		break;
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
	case 'q':
		//center on cursor
		X -= (0.5L-static_cast<long double>(x)/static_cast<long double>(WIDTH)) * spanX;	// dr has to be subtracted here
		Y += (0.5L-static_cast<long double>(y)/static_cast<long double>(HEIGHT)) * spanY;
		setParams(X, Y, spanY, MAXITERS);
		glutPostRedisplay();
		glutWarpPointer(WIDTH/2, HEIGHT/2);	// move cursor to center of window
		break;
	}
}

void handleSpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		X += PIXEL_STRAFE_LD/static_cast<long double>(WIDTH) * spanX;
		setParams(X, Y, spanY, MAXITERS);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		X -= PIXEL_STRAFE_LD/static_cast<long double>(WIDTH) * spanX;
		setParams(X, Y, spanY, MAXITERS);
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		Y -= PIXEL_STRAFE_LD/static_cast<long double>(WIDTH) * spanY;
		setParams(X, Y, spanY, MAXITERS);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		Y += PIXEL_STRAFE_LD/static_cast<long double>(WIDTH) * spanY;
		setParams(X, Y, spanY, MAXITERS);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void inspectPoint(long double rPos, long double iPos)
{
	setParams(rPos, iPos, spanY, MAXITERS);
}
