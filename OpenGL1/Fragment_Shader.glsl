#version 450 core

layout(location = 0) out vec4 out_color;
layout(location = 0) in vec2 position;


uniform float left = -2.2;
uniform float right = 1.0;
uniform float top = 1.4;
uniform float bottom = -1.4;



uniform int maxiters = 100;
uniform float ratio = 1.5;
vec2 point;

void main(void)
{
	float dx = (right - left) / 2.0;									//  =  1.6
	float dy = (top - bottom) / 2.0;									//  =  1.4
	float midx = (right + left) / 2.0;								// = -0.6
	float midy = (top + bottom) / 2.0;								// = 0

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
		out_color = vec4(sin(iteration/10), cos(iteration)/10, 1.0, 0.0);
	}
}