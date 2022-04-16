#pragma once

#include <iostream>
#include <thread>

#include <imgui.h>
#include <Magick++.h>
#include <nlohmann/json.hpp>

#include "helper.hpp"
#include "utility.hpp"

void onNewFile();
void onOpenFile();
void onSaveFile();
void onSaveAsFile();
void onImportFile();
void onExportFile();
void onExitFile();