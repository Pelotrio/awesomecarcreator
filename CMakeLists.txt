cmake_minimum_required (VERSION 3.8)

project ("awesomecarcreator")

add_executable (awesomecarcreator "awesomecarcreator.cpp" "awesomecarcreator.hpp" "dearimgui.cpp" "utility.cpp" "utility.hpp")

target_compile_features(awesomecarcreator PRIVATE cxx_std_20)

find_package(unofficial-graphicsmagick CONFIG REQUIRED)
target_link_libraries(awesomecarcreator PRIVATE unofficial::graphicsmagick::graphicsmagick)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(awesomecarcreator PRIVATE imgui::imgui)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(awesomecarcreator PRIVATE SDL2::SDL2 SDL2::SDL2main)

target_link_libraries(awesomecarcreator PRIVATE opengl32)
