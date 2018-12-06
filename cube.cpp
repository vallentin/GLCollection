
#include <stdlib.h>
#include <stdio.h>

#include "gl.h"
#include "shader.h"
#include "linmath.h"
#include "glfw_utilities.h"

int main(int argc, char *argv[])
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed initializing GLFW\n");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Cube - GLCollection", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed creating window\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glcCenterWindow(window, glcGetBestMonitor(window));

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
			"out vec3 vNormal;\n"
			"\n"
			"in vec3 position;\n"
			"in vec3 normal;\n"
			"\n"
			"uniform mat4 mvp;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    vNormal = normal;\n"
			"    gl_Position = mvp * vec4(position, 1.0);\n"
			"}\n";

	static const GLchar *fragmentShaderSource =
			"#version 330 core\n"
			"\n"
			"out vec4 fragColor;\n"
			"\n"
			"in vec3 vNormal;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    fragColor = vec4(abs(vNormal), 1.0);\n"
			"}\n";

	const GLuint vertexShader   = glcCreateShader(GL_VERTEX_SHADER, vertexShaderSource);
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
	const GLint normalLocation   = glGetAttribLocation(program, "normal");

	const GLint mvpLocation = glGetUniformLocation(program, "mvp");

	if ((positionLocation == -1) || (normalLocation == -1))
		return EXIT_FAILURE;

	static const GLfloat vertices[] = {
			// X, Y, Z, NX, NY, NZ

			// Front
			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top Left
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom Left
			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top Right

			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top Right
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom Left
			 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom Right

			// Back
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Top Left
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom Left
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Top Right

			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Top Right
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom Left
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom Right

			// Top
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top Left
			-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom Left
			 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top Right

			 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top Right
			-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom Left
			 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom Right

			 // Bottom
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, // Top Left
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Bottom Left
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, // Top Right

			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, // Top Right
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Bottom Left
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Bottom Right

			// Right
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top Left
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Bottom Left
			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Top Right

			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Top Right
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Bottom Left
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom Right

			// Left
			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Top Left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Bottom Left
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // Top Right

			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // Top Right
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Bottom Left
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // Bottom Right
	};

	static const GLsizei vertexSize = 6;
	static const GLsizei vertexCount = sizeof(vertices) / sizeof(*vertices) / vertexSize;

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray((GLuint)positionLocation);
	glVertexAttribPointer((GLuint)positionLocation, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), 0);

	glEnableVertexAttribArray((GLuint)normalLocation);
	glVertexAttribPointer((GLuint)normalLocation, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	float model[16], view[16], projection[16];
	float mvp[16];
	float temp[16];

	static const float fov   = 70.0f;
	static const float zNear = 0.01f;
	static const float zFar  = 10.0f;

	while (!glfwWindowShouldClose(window))
	{
		int viewportWidth, viewportHeight;
		glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);

		if (mvpLocation != -1)
		{
			const float aspect = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
			const float time = static_cast<GLfloat>(glfwGetTime());

			mat4Perspective(projection, fov, aspect, zNear, zFar);
			mat4Translation(view, 0.0f, 0.0f, -2.0f);
			mat4Rotation(model, time, 0.0f, 1.0f, 0.0f);

			mat4Multiply(temp, projection, view);
			mat4Multiply(mvp, temp, model);

			glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp);
		}

		glViewport(0, 0, viewportWidth, viewportHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

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
