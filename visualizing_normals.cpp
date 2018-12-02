
#include <stdlib.h>
#include <stdio.h>

#include "gl.h"
#include "shader.h"
#include "linmath.h"

int main(int argc, char *argv[])
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed initializing GLFW\n");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Visualizing Normals - GLCollection", NULL, NULL);

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

	static const GLchar *defaultVertexShaderSource =
			"#version 330 core\n"
			"\n"
			"layout(location = " GLC_STRINGIFY(GLC_ATTRIBUTE_POSITION) ") in vec3 position;\n"
			"layout(location = " GLC_STRINGIFY(GLC_ATTRIBUTE_NORMAL) ") in vec3 normal;\n"
			"\n"
			"out vec3 vNormal;\n"
			"\n"
			"uniform mat4 mvp;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    vNormal = normal;\n"
			"    gl_Position = mvp * vec4(position, 1.0);\n"
			"}\n";

	static const GLchar *defaultFragmentShaderSource =
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

	const GLuint defaultVertexShader   = glcCreateShader(GL_VERTEX_SHADER, defaultVertexShaderSource);
	const GLuint defaultFragmentShader = glcCreateShader(GL_FRAGMENT_SHADER, defaultFragmentShaderSource);

	if ((defaultVertexShader == GLC_NULL_HANDLE) || (defaultFragmentShader == GLC_NULL_HANDLE))
		return EXIT_FAILURE;

	const GLuint defaultProgram = glcCreateProgram(defaultVertexShader, defaultFragmentShader);

	if (defaultProgram == GLC_NULL_HANDLE)
		return EXIT_FAILURE;

	glDeleteShader(defaultFragmentShader);
	glDeleteShader(defaultVertexShader);

	const GLuint visualizeNormalsVertexShader   = glcCreateShaderFromFile(GL_VERTEX_SHADER, "shaders/visualize_normals.vert");
	const GLuint visualizeNormalsFragmentShader = glcCreateShaderFromFile(GL_FRAGMENT_SHADER, "shaders/visualize_normals.frag");
	const GLuint visualizeNormalsGeometryShader = glcCreateShaderFromFile(GL_GEOMETRY_SHADER, "shaders/visualize_normals.geom");

	if ((visualizeNormalsVertexShader == GLC_NULL_HANDLE) || (visualizeNormalsFragmentShader == GLC_NULL_HANDLE) || (visualizeNormalsGeometryShader == GLC_NULL_HANDLE))
		return EXIT_FAILURE;

	const GLuint visualizeNormalsProgram = glcCreateProgram(visualizeNormalsVertexShader, visualizeNormalsFragmentShader, visualizeNormalsGeometryShader);

	const GLint defaultMVPLocation = glGetUniformLocation(defaultProgram, "mvp");
	const GLint visualizeNormalsMVPLocation = glGetUniformLocation(visualizeNormalsProgram, "mvp");

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

	glEnableVertexAttribArray(GLC_ATTRIBUTE_POSITION);
	glVertexAttribPointer(GLC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(GLC_ATTRIBUTE_NORMAL);
	glVertexAttribPointer(GLC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));

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

		const float aspect = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
		const float time = static_cast<GLfloat>(glfwGetTime());

		mat4Perspective(projection, fov, aspect, zNear, zFar);

		mat4Identity(view);
		mat4Translate(view, 0.0f, 0.0f, -3.0f);
		mat4Rotate(view, GLC_RAD(cosf(time * 0.75f) * 16.0f), 1.0f, 0.0f, 0.0f);

		mat4Rotation(model, time * 0.5f, 0.0f, 1.0f, 0.0f);

		mat4Multiply(temp, projection, view);
		mat4Multiply(mvp, temp, model);

		glViewport(0, 0, viewportWidth, viewportHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(defaultProgram);
		glUniformMatrix4fv(defaultMVPLocation, 1, GL_FALSE, mvp);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glUseProgram(visualizeNormalsProgram);
		glUniformMatrix4fv(visualizeNormalsMVPLocation, 1, GL_FALSE, mvp);
		glDrawArrays(GL_POINTS, 0, vertexCount);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glDeleteProgram(defaultProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
