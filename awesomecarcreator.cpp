#include "awesomecarcreator.hpp"

struct {
	bool jpegify_activate = true;
	int jpegify_power = 92;
	int jpegify_repetitions = 50;

	bool ruin_quality_activate = true;
	int ruin_quality_power = 3;

	bool change_color_activate = true;
	int change_color_mode = 0;
	bool change_color_use_image = false;
	ImVec4 change_color_car_color;
	ImVec4 change_color_background_color;
} generator_settings;

int main(int argc, char* argv[])
{
	Magick::InitializeMagick(*argv);

	GLFWwindow* window = helper::imgui_init(1280, 720, "awesomecarcreator");

	ImGui::StyleColorsDark();
	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiStyle* style = &ImGui::GetStyle();

	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\calibri.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\consola.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\consolab.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\arial.ttf)", 18.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\comic.ttf)", 24.0f);
	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\times.ttf)", 18.0f);

	/*=========================================================*/

	Magick::Image image;

	image.read(R"(C:\image.png)");

	GLuint asdf = helper::create_texture();

	helper::image_to_texture(image, asdf);

	/*=========================================================*/

	bool show_demo_window = true;
	bool show_metrics_window = false;

	bool show_about_window = false;

	while (helper::loop(window))
	{
		helper::begin_frame();

		/*==========*/


		//"Workspace"
		{

			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);

			ImGui::Begin("Workspace", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

			float verticalSize = viewport->WorkSize.y - style->WindowPadding.y * 2;

			if (ImGui::BeginTable("Workspace split", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBodyUntilResize))
			{
				ImGui::TableNextColumn();

				//"Left sidebar"
				{
					ImGui::BeginChild("Left sidebar", ImVec2(0, verticalSize), true);

					ImGui::Checkbox("JPEGify", &generator_settings.jpegify_activate);

					if (generator_settings.jpegify_activate)
					{
						ImGui::SliderInt("Power", &generator_settings.jpegify_power, 0, 100);
						ImGui::SliderInt("Repetitions", &generator_settings.jpegify_repetitions, 10, 120);
					}

					ImGui::Separator();

					ImGui::Checkbox("Ruin quality", &generator_settings.ruin_quality_activate);

					if (generator_settings.ruin_quality_activate)
					{
						ImGui::SliderInt("Power##ruin_quality", &generator_settings.ruin_quality_power, 2, 5);
					}

					ImGui::Separator();

					ImGui::Checkbox("Change color", &generator_settings.change_color_activate);

					if (generator_settings.change_color_activate && ImGui::BeginTable("Padding##change_color_mode", 2))
					{
						ImGui::TableNextColumn();

						ImGui::RadioButton("Color bake mode", &generator_settings.change_color_mode, 0);

						ImGui::TableNextColumn();

						ImGui::RadioButton("Overlay mode", &generator_settings.change_color_mode, 1);

						ImGui::EndTable();

						ImGui::Checkbox("Use image", &generator_settings.change_color_use_image);

						ImGui::SameLine();

						ImGui::BeginDisabled(!generator_settings.change_color_use_image);

						static char asdf[3000] = { 0 };
						ImGui::InputText("##file_location", asdf, 3000);

						ImGui::EndDisabled();

						if (!generator_settings.change_color_use_image && ImGui::BeginTable("Padding", 2))
						{
							ImGui::TableSetupColumn("Car");
							ImGui::TableSetupColumn("Background");
							ImGui::TableHeadersRow();

							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);

							ImGui::SetNextItemWidth(-FLT_MIN);
							ImGui::ColorPicker3("Car", (float*)&generator_settings.change_color_car_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputRGB);

							ImGui::TableNextColumn();

							ImGui::SetNextItemWidth(-FLT_MIN);
							ImGui::ColorPicker3("Background", (float*)&generator_settings.change_color_background_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputRGB);

							ImGui::EndTable();
						}
					}

					ImGui::EndChild();
				}

				ImGui::TableNextColumn();

				//"Center"
				{
					ImGui::BeginChild("Center", ImVec2(0, verticalSize), false);

					ImGui::Image((ImTextureID)asdf, ImVec2(std::min(ImGui::GetContentRegionAvail().x, verticalSize), std::min(ImGui::GetContentRegionAvail().x, verticalSize)));

					ImGui::EndChild();
				}

				ImGui::TableNextColumn();

				//"Right sidebar"
				{
					ImGui::BeginChild("Right sidebar", ImVec2(0, verticalSize), true);

					ImGui::Checkbox("Demo Window", &show_demo_window);
					ImGui::Checkbox("Metrics Window", &show_metrics_window);

					ImGui::Separator();

					ImGui::ShowFontSelector("Default Font");

					ImGui::Separator();

					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

					ImGui::EndChild();
				}

				ImGui::EndTable();
			}

			ImGui::End();
		}

		if (show_about_window)
		{
			ImGui::Begin("About", &show_about_window);

			ImGui::Text("Nothing here yet :(");

			ImGui::End();
		}

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_metrics_window)
			ImGui::ShowMetricsWindow(&show_metrics_window);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) { onNewFile(); }
				if (ImGui::MenuItem("Open", "Ctrl+O")) { onOpenFile(); }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { onSaveFile(); }
				if (ImGui::MenuItem("Save as", "Ctrl+Shift+S")) { onSaveAsFile(); }
				ImGui::Separator();
				if (ImGui::MenuItem("Import", "Ctrl+I")) { onImportFile(); }
				if (ImGui::MenuItem("Export", "Ctrl+E")) { onExportFile(); }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "Alt+F4")) { onExitFile(); }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::Text("Nothing here yet :(");

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::Text("Nothing here yet :(");

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("About", 0, &show_about_window);
				ImGui::Separator();
				ImGui::Text("awesomecarcreator 0.0.1a");

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		//TODO: keybinds


		/*==========*/

		helper::end_frame(window, style->Colors[ImGuiCol_DockingEmptyBg]);
	}

	helper::cleanup(window);

	return 0;
}

void onNewFile()
{
	std::cout << "onNewFile() called" << std::endl;
}

void onOpenFile()
{
	std::cout << "onOpenFile() called" << std::endl;
}

void onSaveFile()
{
	std::cout << "onSaveFile() called" << std::endl;
}

void onSaveAsFile()
{
	std::cout << "onSaveAsFile() called" << std::endl;
}

void onImportFile()
{
	std::cout << "onImportFile() called" << std::endl;
}

void onExportFile()
{
	std::cout << "onExportFile() called" << std::endl;
}

void onExitFile()
{
	std::cout << "onExitFile() called" << std::endl;

	exit(0);
}