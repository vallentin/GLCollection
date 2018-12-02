#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

out vec3 vNormal;

void main()
{
	vNormal = normal;

	gl_Position = vec4(position, 1.0);
}
