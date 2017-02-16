#version 420 core
 
layout(location = 0) out vec2 position;

void main(void)
{
	const vec4 vertices[4] = vec4[4](	vec4( -1.0, -1.0, 0.0, 1.0),
										vec4( 1.0, -1.0, 0.0, 1.0),
										vec4( 1.0, 1.0, 0.0, 1.0),
										vec4( -1.0, 1.0, 0.0, 1.0)); 
	position = vec2(vertices[gl_VertexID][0], vertices[gl_VertexID][1]);
	gl_Position = vertices[gl_VertexID];
/*id:1x1x1*/}