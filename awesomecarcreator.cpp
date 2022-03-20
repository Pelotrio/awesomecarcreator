#include "awesomecarcreator.hpp"

int main(int argc, char* argv[])
{
	Magick::InitializeMagick(*argv);

	GLFWwindow* window = helper::imgui_init(std::bind(ImGui::StyleColorsDark, nullptr));

	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\Arial.ttf)", 16.0f);

	/*=========================================================*/

	Magick::Image image;

	image.read(R"(C:\image.png)");

	utility::generate_video(image);

	/*=========================================================*/

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool show_demo_window = true;
	bool show_another_window = false;

	static float f = 0.0f;
	static int counter = 0;

	while (helper::loop(window))
	{
		helper::begin_frame();

		/*==========*/


		//"Hello, world!" window
		{
			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("Another Window", &show_another_window);
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			ImGui::End();
		}
		

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_another_window)
		{
			//"Another window" window
			{
				ImGui::Begin("Another Window", &show_another_window);

				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;

				ImGui::End();
			}
		}
		

		/*==========*/

		helper::end_frame(window, clear_color);
	}

	helper::cleanup(window);

	return 0;
}
