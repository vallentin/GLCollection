
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "gl.h"
#include "glfw_utilities.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h> // https://github.com/nothings/stb

void flipVertically(int width, int height, char *data)
{
	char rgb[3];

	for (int y = 0; y < height / 2; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int top = (x + y * width) * 3;
			int bottom = (x + (height - y - 1) * width) * 3;

			memcpy(rgb, data + top, sizeof(rgb));
			memcpy(data + top, data + bottom, sizeof(rgb));
			memcpy(data + bottom, rgb, sizeof(rgb));
		}
	}
}

int saveScreenshot(const char *filename)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x = viewport[0];
	int y = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	char *data = (char*) malloc((size_t) (width * height * 3)); // 3 components (R, G, B)

	if (!data)
		return 0;

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	flipVertically(width, height, data);

	int saved = stbi_write_png(filename, width, height, 3, data, 0);

	free(data);

	return saved;
}

const char* createScreenshotBasename()
{
	static char basename[30];

	time_t t = time(NULL);
	strftime(basename, 30, "%Y%m%d_%H%M%S.png", localtime(&t));

	return basename;
}

int captureScreenshot()
{
	char filename[50];

	strcpy(filename, "screenshots/");
	strcat(filename, createScreenshotBasename());

	int saved = saveScreenshot(filename);

	if (saved)
		printf("Successfully Saved Image: %s\n", filename);
	else
		fprintf(stderr, "Failed Saving Image: %s\n", filename);

	return saved;
}

int main(int argc, char *argv[])
{
	// flipVertically in saveScreenshot
	// or
	// stbi_flip_vertically_on_write(1);

	if (!glfwInit())
	{
		fprintf(stderr, "Failed initializing GLFW\n");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Screenshot - GLCollection", NULL, NULL);

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

	glEnable(GL_SCISSOR_TEST);

	bool repeated = false;

	while (!glfwWindowShouldClose(window))
	{
		int down = glfwGetKey(window, GLFW_KEY_F5);

		if (down && !repeated)
		{
			captureScreenshot();

			repeated = true;
		}
		else if (!down)
			repeated = false;

		int viewportWidth, viewportHeight;
		glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);

		glViewport(0, 0, viewportWidth, viewportHeight);

		glScissor(0, viewportHeight / 2, viewportWidth / 2, viewportHeight / 2);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glScissor(viewportWidth / 2, viewportHeight / 2, viewportWidth / 2, viewportHeight / 2);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glScissor(0, 0, viewportWidth / 2, viewportHeight / 2);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glScissor(viewportWidth / 2, 0, viewportWidth / 2, viewportHeight / 2);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
