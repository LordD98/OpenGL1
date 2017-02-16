#include "main.h"


int main(int argc, char **argv)
{
	int numOfBaseColors = 5;
	struct floatColor *baseColors = (struct floatColor*)calloc(numOfBaseColors, sizeof(struct floatColor));
	baseColors[0] = newFloatColor(0.0F, 0.0F, 1.0F);
	baseColors[1] = newFloatColor(0.3F, 0.2F, 0.7F);
	baseColors[2] = newFloatColor(1.0F, 0.5F, 0.0F);
	baseColors[3] = newFloatColor(0.6F, 0.5F, 0.5F);
	baseColors[4] = newFloatColor(0.0F, 0.0F, 1.0F); 
	iterationSpan = (numOfBaseColors-1)*10 + 1;
	generateColorTable(iterationSpan, baseColors, numOfBaseColors);
	//float *ntable = (float *)memcpy(ntable,table, iterationSpan*3*sizeof(float));
	presentColorTable(table, iterationSpan);
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
	printf("Sucessfully rendered frame!\n");
}

void resize (int w, int h)
{
	glViewport(0, 0, w, h);
	ratio = (long double)w / h;
	printf("redrawing!\n");
	renderScene();
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
		glUniform3fv(loc, iterationSpan, table);    //2,9,6 =^= r,g,b
	}

	loc = glGetUniformLocation(program, "iterationSpan");
	if (loc != -1)
	{
		glUniform1i(loc, iterationSpan);
	}
}

void Init()
{
	glEnable(GL_DEPTH_TEST);

	//load and compile shaders
	program = CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl", iterationSpan);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void menuCallback(int id) 
{
	printf("%i\n", id);
}

void generateColorTable(int iterationSpan, struct floatColor *baseColors, int numOfBaseColors)
{
	table = (GLfloat*)calloc(3 * iterationSpan, sizeof(GLfloat));		//...< ... .<.. .<.. ...
	int lastColorIndex = numOfBaseColors - 1;
	int numOfIntervs = numOfBaseColors - 1;
	if ((iterationSpan-1) % numOfIntervs == 0)			//numOfIntervs muss iterationSpan-1 teilen!!! 
	{
		int lengthOfInterv = (iterationSpan - 1) / numOfIntervs;
		/*0 iterations =^= colors[0]
		<iterationSpan> iteration =^= colors[end]
		.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//iterationSpan = 16 = 15 + 1
		,			,			,			,			,			,	//numberOfBaseColors = 6;	5 * interv(3)
		,					,					,					,	//numberOfBaseColors = 4;	3 * interv(5)
	
		.	.	.	.	.	.	.	.	.	.	.	.	.				//iterationSpan = 13
		.			.			.			.			.				//numOfBaseColors = 5; ninterv = nbasecolor
		*/
		{
			float rDiff, gDiff, bDiff, r, g, b, intervallAdvancement;		//All variables declared
			struct floatColor startBaseColor, endBaseColor;						//before the nested loop
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

struct floatColor newFloatColor(float r, float g, float b)
{
	struct floatColor color;
	color.x = r;
	color.y = g;
	color.z = b;
	return color;
}

void presentColorTable(GLfloat *table, int iterationSpan)
{
	for (int i = 0; i<iterationSpan; i++)
	{
		printf("%f %f %f\n", table[i * 3], table[i * 3 + 1], table[i * 3 + 2]);
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
		//glutReshapeFunc(resize);		//shure
		//glutKeyboardFunc(handleKey);	//on
		//glutMainLoop();				//these
		break;
	case ' ':
		break;
		
	}
	//int temp = glutGetModifiers();			//shift 1, ctrl 2, alt 4   (sum if more than one)
	int CTRL_DOWN = glutGetModifiers() & GLUT_ACTIVE_CTRL;		//bitwise and to filter for ctrl-part
}

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
		printf("Can't read file %s \n", filename);
		terminate();
	}
	return buffer;
}

GLuint CreateShader(GLenum shaderType, char* source, char* shaderName)
{

	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source;	
	const int shader_code_size = strlen(source);

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		char* shader_log = (char*)malloc(info_log_length*sizeof(char));
		glGetShaderInfoLog(shader, info_log_length, NULL, shader_log);
		printf("ERROR compiling shader: %s\n%s\n", shaderName, shader_log);
		free(shader_log);
		return 0;
	}
	return shader;
}

GLuint CreateProgram(char* vertexShaderFilename, char* fragmentShaderFilename, int iterationSpan)
{

	//read the shader files and save the code
	char* vertex_shader_code = ReadShader(vertexShaderFilename);
	char* fragment_shader_code = ReadShader(fragmentShaderFilename);

	//modify the shader code with regex to match the number of iterations contained in the table
	//also do other things with the shader code#

	const char *err_msg;
	int err;
	int occs[2];

	char *pattern = "\\/\\*id:1x1x1\\*\\/}";
	pcre *re = pcre_compile(pattern, 0, &err_msg, &err, NULL);
	pcre_exec(re, NULL, vertex_shader_code, strlen(vertex_shader_code), 0, 0, occs, 2);	//offset[0] = 10(index)
	pcre_free(re);
	char *str2 = (char*)malloc((occs[1] + 1) * sizeof(char));					//=> 10 chars, so up to "0"
	memcpy(str2, vertex_shader_code, occs[1] * sizeof(char));						//str1[10] = "a"
	str2[occs[1]] = '\0';

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, str2, "vertex shader");
	free(str2);

	pattern = "\\/\\*id:1x1x2\\*\\/}";
	re = NULL;

	re = pcre_compile(pattern, 0, &err_msg, &err, NULL);
	pcre_exec(re, NULL, fragment_shader_code, strlen(fragment_shader_code), 0, 0, occs, 2);	//offset[0] = 10(index)
	pcre_free(re);
	str2 = (char*)malloc((occs[1] + 1) * sizeof(char));					//=> 10 chars, so up to "0"
	memcpy(str2, fragment_shader_code, occs[1] * sizeof(char));						//str1[10] = "a"
	str2[occs[1]] = '\0';

	char replacement[50] = "uniform vec3 colorTable[";
	int len = floor(log10(iterationSpan)) + 1;
	char *s = (char*)malloc(len*sizeof(char));
	sprintf(s, "%i", iterationSpan);
	strcat(replacement, s);
	strcat(replacement, "];");

	pattern = "uniform vec3 colorTable\\[[0-9]*];";
	re = NULL;
	re = pcre_compile(pattern, 0, &err_msg, &err, NULL);
	pcre_exec(re, NULL, str2, strlen(str2), 0, 0, occs, 2);
	pcre_free(re);
	//fragment_shader_code = regex_replace(fragment_shader_code, reg1, "uniform vec3 colorTable[" + to_char*(iterationSpan) + "];");

	char *temp;
	char *newstr;
	temp = (char*)malloc((strlen(str2) - occs[1] + 1) * sizeof(char));
	strcpy(temp, &str2[occs[1]]);
	newstr = (char*)calloc(occs[0] + strlen(temp) + 1 + strlen(replacement), sizeof(char));
	strncat(newstr, str2, (size_t)occs[0]);
	strcat(newstr, replacement);		//copy the new string to occs[0]
	strcat(newstr, temp);				//copy the copy temp to occs[0] + strlen()
	str2 = newstr;

	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, str2, "fragment shader");

	free(newstr);
	free(temp);


	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		char * program_log = (char*)malloc(info_log_length*sizeof(char));
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		printf("Shader Loader : LINK ERROR\n%s\n", &program_log[0]);
		return 0;
	}
	return program;
}