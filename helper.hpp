#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <functional>

namespace helper
{
	bool loop(GLFWwindow* window);
	void begin_frame();
	void end_frame(GLFWwindow* window, ImVec4 clear_color);
	static void GLFW_error_callback(int error, const char* description);
	GLFWwindow* imgui_init(std::function<void()> ImGui_style);
	void cleanup(GLFWwindow* window);
}