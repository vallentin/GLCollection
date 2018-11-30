#ifndef GLC_SHADER_H
#define GLC_SHADER_H

#include <stdio.h>

#include "gl.h"

const char* glcGetShaderTypeString(GLenum type)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		return "Vertex";
	case GL_FRAGMENT_SHADER:
		return "Fragment";
	case GL_GEOMETRY_SHADER:
		return "Geometry";
	case GL_TESS_CONTROL_SHADER:
		return "TessellationControl";
	case GL_TESS_EVALUATION_SHADER:
		return "TessellationEvaluation";
	case GL_COMPUTE_SHADER:
		return "Compute";
	default:
		return "Unknown";
	}
}

void glcCheckShaderLog(GLuint shader)
{
	if (shader == GLC_NULL_HANDLE)
		return;

	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength)
	{
		GLint type;
		glGetShaderiv(shader, GL_SHADER_TYPE, &type);

		GLchar *log = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, &logLength, log);

		printf("%s Shader Log: %s\n", glcGetShaderTypeString((GLenum)type), log);

		delete[] log;
	}
}

void glcCheckProgramLog(GLuint program)
{
	if (program == GLC_NULL_HANDLE)
		return;

	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength)
	{
		GLchar *log = new GLchar[logLength];
		glGetProgramInfoLog(program, logLength, &logLength, log);

		printf("Program Log: %s\n", log);

		delete[] log;
	}
}

GLuint glcCreateShader(GLenum type, const GLchar *source)
{
	GLuint shader = glCreateShader(type);

	if (shader == GLC_NULL_HANDLE)
		return GLC_NULL_HANDLE;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	glcCheckShaderLog(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status)
		return shader;

	glDeleteShader(shader);

	return GLC_NULL_HANDLE;
}

GLuint glcCreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint program = glCreateProgram();

	if (program == GLC_NULL_HANDLE)
		return GLC_NULL_HANDLE;

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glBindFragDataLocation(program, 0, "fragColor");

	glLinkProgram(program);
	glcCheckProgramLog(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (!status)
	{
		glDeleteProgram(program);
		return GLC_NULL_HANDLE;
	}

	glValidateProgram(program);
	glcCheckProgramLog(program);

	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

	if (!status)
	{
		glDeleteProgram(program);
		return GLC_NULL_HANDLE;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

#endif