
#include <stdlib.h>
#include <stdio.h>

#include "gl.h"
#include "shader.h"

int main(int argc, char *argv[])
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed initializing GLFW\n");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Screen Quad - GLCollection", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed creating window\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwShowWindow(window);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed loading OpenGL functions and extensions\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	static const GLchar *vertexShaderSource =
			"#version 330 core\n"
			"\n"
			"in vec2 position;\n"
			"in vec2 texCoord;\n"
			"\n"
			"out vec2 vTexCoord;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    vTexCoord = texCoord;\n"
			"    gl_Position = vec4(position, 0.0, 1.0);\n"
			"}\n";

	static const GLchar *fragmentShaderSource =
			"#version 330 core\n"
			"\n"
			"out vec4 fragColor;\n"
			"\n"
			"in vec2 vTexCoord;\n"
			"\n"
			"uniform float time;\n"
			"\n"
			"void main()\n"
			"{\n"
			// "    fragColor = vec4(vTexCoord, 0.0, 1.0);\n"
			"    fragColor = vec4(sin(time + vTexCoord.xyx + vec3(0.0, 2.0, 4.0)) * 0.5 + 0.5, 1.0);\n"
			"}\n";

	const GLuint vertexShader = glcCreateShader(GL_VERTEX_SHADER, vertexShaderSource);
	const GLuint fragmentShader = glcCreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	if ((vertexShader == GLC_NULL_HANDLE) || (fragmentShader == GLC_NULL_HANDLE))
		return EXIT_FAILURE;

	const GLuint program = glcCreateProgram(vertexShader, fragmentShader);

	if (program == GLC_NULL_HANDLE)
		return EXIT_FAILURE;

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glUseProgram(program);

	const GLint positionLocation = glGetAttribLocation(program, "position");
	const GLint texCoordLocation = glGetAttribLocation(program, "texCoord");

	const GLint timeLocation = glGetUniformLocation(program, "time");

	if ((positionLocation == -1) || (texCoordLocation == -1))
		return EXIT_FAILURE;

	static const GLfloat vertices[] = {
			// X, Y, U, V
			-1.0f,  1.0f, 0.0f, 1.0f, // Top Left
			-1.0f, -1.0f, 0.0f, 0.0f, // Bottom Left
			 1.0f, -1.0f, 1.0f, 0.0f, // Bottom Right
			 1.0f,  1.0f, 1.0f, 1.0f, // Top Right
	};

	static const GLsizei vertexSize = 4;
	static const GLsizei vertexCount = sizeof(vertices) / sizeof(*vertices) / vertexSize;

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray((GLuint)positionLocation);
	glVertexAttribPointer((GLuint)positionLocation, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), 0);

	glEnableVertexAttribArray((GLuint)texCoordLocation);
	glVertexAttribPointer((GLuint)texCoordLocation, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(2 * sizeof(GLfloat)));

	while (!glfwWindowShouldClose(window))
	{
		int viewportWidth, viewportHeight;
		glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);

		glViewport(0, 0, viewportWidth, viewportHeight);

		if (timeLocation != -1)
			glUniform1f(timeLocation, static_cast<GLfloat>(glfwGetTime()));

		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glDeleteProgram(program);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
