#version 420 core

vec3 getColor(int iterations);

layout(location = 0) out vec4 out_color;
layout(location = 0) in vec2 position;

uniform vec4 rect; // vec4(-2.7, 1.0, 1.4, -1.4)

uniform float left;				// = -2.7;
uniform float right;			// = 1.0;
uniform float top;				// = 1.4;
uniform float bottom;			// = -1.4;

uniform int maxiters;			// = 100;


vec2 point;

void main(void)
{
	float dx = (rect.y - rect.x) / 2.0;					//(right - left) / 2.0  =  1.6
	float dy = (rect.z - rect.w) / 2.0;					//(top - bottom) / 2.0  =  1.4
	float midx = (rect.y + rect.x) / 2.0;				//(right + left) / 2.0  = -0.6
	float midy = (rect.z + rect.w) / 2.0;				//(top + bottom) / 2.0  =  0
	point = vec2((position[0] * dx) + midx, (position[1] * dy) + midy);
	
	float temp;
	int iteration = 0;
	vec2 val = point;
	while(iteration<maxiters)             //actual iteration-count is i-1 at the end
	{
		if(val[0]*val[0] + val[1]*val[1] > 4)
			break;
		else
			iteration++;
		temp = val[0];
		val[0] = val[0]*val[0] - val[1]*val[1] + point[0];
		val[1] = 2*temp*val[1] + point[1];
	}

	if(iteration == maxiters)
	{
		out_color = vec4(0.0, 0.0, 0.0, 1.0);
	}	
	else
	{
		out_color = vec4(getColor(iteration), 1.0);
	}
}

vec3 getColor(int iterations)
{
	return vec3(sin(iterations/10), cos(iterations)/10, 1.0);
}