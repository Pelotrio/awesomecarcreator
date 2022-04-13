#include "helper.hpp"

namespace helper
{
	bool loop(GLFWwindow* window)
	{
		return !glfwWindowShouldClose(window);
	}

	void begin_frame()
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
	}

	void end_frame(GLFWwindow* window, ImVec4 clear_color)
	{
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	static void GLFW_error_callback(int error, const char* description)
	{
		std::cerr << std::hex << "GLFW error " << error << " occurred: " << description << std::endl;
	}

	GLFWwindow* imgui_init(std::function<void()> ImGui_style)
	{
		glfwSetErrorCallback(GLFW_error_callback);

		if (!glfwInit())
			return nullptr;

		#if defined(IMGUI_IMPL_OPENGL_ES2)
		const char* glsl_version = "#version 100"; //GL ES 2.0 + GLSL 100
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		#elif defined(__APPLE__)
		const char* glsl_version = "#version 150"; //GL 3.2 + GLSL 150
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#else
		const char* glsl_version = "#version 130"; //GL 3.0 + GLSL 130
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		#endif

		GLFWwindow* window = glfwCreateWindow(1280, 720, "awesomecarcreator", NULL, NULL);

		if (window == NULL)
			return nullptr;

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui_style();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		return window;
	}

	void cleanup(GLFWwindow* window)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	GLuint create_texture()
	{
		GLuint texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//TODO:
		//This is required on WebGL for non power-of-two textures
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		#endif

		return texture;
	}

	void image_to_texture(Magick::Image image, GLuint texture)
	{
		Magick::Geometry size = image.size();

		Magick::Blob temp;
		image.write(&temp, "RGB");

		glBindTexture(GL_TEXTURE_2D, texture);

		//TODO: find method to detect if texture was already defined with glTexImage2D to use glTexSubImage2D (faster)
		if (true)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width(), size.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, temp.data());
		else
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width(), size.height(), GL_RGB, GL_UNSIGNED_BYTE, temp.data());
	}
}
