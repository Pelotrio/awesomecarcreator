#include "awesomecarcreator.hpp"

int main(int argc, char* argv[])
{
	Magick::InitializeMagick(*argv);

	GLFWwindow* window = helper::imgui_init(std::bind(ImGui::StyleColorsDark, nullptr));

	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\calibri.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\consola.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\consolab.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\arial.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\comic.ttf)", 24.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\times.ttf)", 18.0f);

	/*=========================================================*/

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool show_demo_window = true;
	bool show_metrics_window = false;

	static float f = 0.0f;
	static int counter = 0;

	while (helper::loop(window))
	{
		helper::begin_frame();

		/*==========*/


		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save as", "Ctrl+Shift+S")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Import", "Ctrl+I")) {}
				if (ImGui::MenuItem("Export", "Ctrl+E")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "Alt+F4")) {}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::IsKeyPressed(ImGuiKeyModFlags_Ctrl | ImGuiKey_N, false))
			std::cout << "Ctrl+N pressed." << std::endl;

		// ...

		//"Primary Window"
		{
			ImGui::Begin("Primary Window");

			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("Metrics Window", &show_metrics_window);

			ImGui::Separator();

			ImGui::ColorEdit3("Background Color", (float*)&clear_color);
			ImGui::ShowFontSelector("Default Font");

			ImGui::Separator();

			ImGui::Text("counter = %d", counter);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			ImGui::End();
		}

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_metrics_window)
			ImGui::ShowMetricsWindow(&show_metrics_window);


		/*==========*/

		helper::end_frame(window, clear_color);
	}

	helper::cleanup(window);

	return 0;
}