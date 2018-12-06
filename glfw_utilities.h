#ifndef GLC_GLFW_UTILITIES_H
#define GLC_GLFW_UTILITIES_H

#include "gl.h"

#define GLC_MAX(a, b) (((a) < (b)) ? (b) : (a))
#define GLC_MIN(a, b) (((b) < (a)) ? (b) : (a))

void glcCenterWindow(GLFWwindow *window, GLFWmonitor *monitor)
{
	if (!monitor)
		return;

	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	if (!mode)
		return;

	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window,
	                 monitorX + (mode->width - windowWidth) / 2,
	                 monitorY + (mode->height - windowHeight) / 2);
}

GLFWmonitor* glcGetBestMonitor(GLFWwindow *window)
{
	int monitorCount;
	GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

	if (!monitors)
		return NULL;

	int windowX, windowY, windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glfwGetWindowPos(window, &windowX, &windowY);

	GLFWmonitor *bestMonitor = NULL;
	int bestArea = 0;

	for (int i = 0; i < monitorCount; ++i)
	{
		GLFWmonitor *monitor = monitors[i];

		int monitorX, monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		if (!mode)
			continue;

		int areaMinX = GLC_MAX(windowX, monitorX);
		int areaMinY = GLC_MAX(windowY, monitorY);

		int areaMaxX = GLC_MIN(windowX + windowWidth, monitorX + mode->width);
		int areaMaxY = GLC_MIN(windowY + windowHeight, monitorY + mode->height);

		int area = (areaMaxX - areaMinX) * (areaMaxY - areaMinY);

		if (area > bestArea)
		{
			bestArea = area;
			bestMonitor = monitor;
		}
	}

	return bestMonitor;
}

#endif