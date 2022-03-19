cmake_minimum_required (VERSION 3.8)

project ("awesomecarcreator")

add_executable (awesomecarcreator "awesomecarcreator.cpp" "awesomecarcreator.hpp" "utility.cpp" "utility.hpp" "helper.cpp" "helper.hpp")

target_compile_features(awesomecarcreator PRIVATE cxx_std_20)

#Hide console window
if ((${CMAKE_SYSTEM_NAME} STREQUAL Windows AND MSVC) AND ((${CMAKE_BUILD_TYPE} STREQUAL Release) OR (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo) OR (${CMAKE_BUILD_TYPE} STREQUAL MinSizeRel)))
	target_link_options (awesomecarcreator PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif ()

#vcpkg install GraphicsMagick:x86-windows GraphicsMagick:x64-windows
find_package (unofficial-graphicsmagick CONFIG REQUIRED)
target_link_libraries (awesomecarcreator PRIVATE unofficial::graphicsmagick::graphicsmagick)

#vcpkg install nlohmann-json:x86-windows nlohmann-json:x64-windows
find_package (nlohmann_json CONFIG REQUIRED)
target_link_libraries (awesomecarcreator PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

#vcpkg install imgui[wchar32,opengl3-binding,glfw-binding,freetype]:x86-windows imgui[wchar32,opengl3-binding,glfw-binding,freetype]:x64-windows
find_package (imgui CONFIG REQUIRED)
target_link_libraries (awesomecarcreator PRIVATE imgui::imgui)

#OpenGL3
find_package (OpenGL REQUIRED)
include_directories (${OPENGL_INCLUDE_DIRS})
target_link_libraries (awesomecarcreator PRIVATE OpenGL::GL)

#GLFW
target_link_libraries(awesomecarcreator PRIVATE glfw)



#vcpkg install GraphicsMagick:x86-windows GraphicsMagick:x64-windows nlohmann-json:x86-windows nlohmann-json:x64-windows imgui[wchar32,opengl3-binding,glfw-binding,freetype]:x86-windows imgui[wchar32,opengl3-binding,glfw-binding,freetype]:x64-windows