#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

out vec3 vColor;

in vec3 vNormal[];

uniform mat4 mvp;
uniform float length = 1.0;

void main()
{
	vec3 normal = vNormal[0];

	vColor = abs(normal);

	vec4 v0 = gl_in[0].gl_Position;
	gl_Position = mvp * v0;
	EmitVertex();

	vec4 v1 = v0 + vec4(normal * length, 0.0);
	gl_Position = mvp * v1;
	EmitVertex();

	EndPrimitive();
}
