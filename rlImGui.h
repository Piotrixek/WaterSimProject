#pragma once

#include "imgui.h"
#include "raylib.h"

void rlImGuiSetup(bool dark);
void rlImGuiShutdown();
void rlImGuiBegin();
void rlImGuiEnd();

void rlImGuiImage(const Texture* image);