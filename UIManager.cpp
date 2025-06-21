#include "UIManager.hpp"
#include "rlImGui.h"
#include "imgui.h"


UIManager::UIManager() {}


void UIManager::Setup() { rlImGuiSetup(true); }
void UIManager::Shutdown() { rlImGuiShutdown(); }
void UIManager::Begin() { rlImGuiBegin(); }
void UIManager::End() { rlImGuiEnd(); }
const WaterParams& UIManager::GetParams() const { return params; }


void UIManager::DrawControls() {
    ImGui::Begin("Water Controls");


    if (ImGui::CollapsingHeader("Wave Layer 1 (Swells)")) {
        ImGui::SliderFloat2("Direction##1", &params.waveDir1.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Steepness##1", &params.steepness1, 0.0f, 1.0f);
        ImGui::SliderFloat("Wavelength##1", &params.wavelength1, 1.0f, 50.0f);
        ImGui::SliderFloat("Speed##1", &params.speed1, 0.0f, 5.0f);
    }
    if (ImGui::CollapsingHeader("Wave Layer 2 (Medium Waves)")) {
        ImGui::SliderFloat2("Direction##2", &params.waveDir2.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Steepness##2", &params.steepness2, 0.0f, 1.0f);
        ImGui::SliderFloat("Wavelength##2", &params.wavelength2, 1.0f, 20.0f);
        ImGui::SliderFloat("Speed##2", &params.speed2, 0.0f, 5.0f);
    }
    if (ImGui::CollapsingHeader("Wave Layer 3 (Ripples)")) {
        ImGui::SliderFloat2("Direction##3", &params.waveDir3.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Steepness##3", &params.steepness3, 0.0f, 1.0f);
        ImGui::SliderFloat("Wavelength##3", &params.wavelength3, 1.0f, 20.0f);
        ImGui::SliderFloat("Speed##3", &params.speed3, 0.0f, 5.0f);
    }
    if (ImGui::CollapsingHeader("Wave Layer 4 (Details)")) {
        ImGui::SliderFloat2("Direction##4", &params.waveDir4.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Steepness##4", &params.steepness4, 0.0f, 1.0f);
        ImGui::SliderFloat("Wavelength##4", &params.wavelength4, 1.0f, 20.0f);
        ImGui::SliderFloat("Speed##4", &params.speed4, 0.0f, 5.0f);
    }

    if (ImGui::CollapsingHeader("Appearance")) {
        float deep[3] = { params.deepColor.r / 255.f, params.deepColor.g / 255.f, params.deepColor.b / 255.f };
        if (ImGui::ColorEdit3("Deep Color", deep)) {
            params.deepColor = { (unsigned char)(deep[0] * 255), (unsigned char)(deep[1] * 255), (unsigned char)(deep[2] * 255), 255 };
        }
        float shallow[3] = { params.shallowColor.r / 255.f, params.shallowColor.g / 255.f, params.shallowColor.b / 255.f };
        if (ImGui::ColorEdit3("Shallow Color", shallow)) {
            params.shallowColor = { (unsigned char)(shallow[0] * 255), (unsigned char)(shallow[1] * 255), (unsigned char)(shallow[2] * 255), 255 };
        }
        ImGui::SliderFloat("Color Bias", &params.colorBias, 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &params.specularFactor, 2.0f, 512.0f);


        ImGui::SeparatorText("Foam");
        float foam[3] = { params.foamColor.r / 255.f, params.foamColor.g / 255.f, params.foamColor.b / 255.f };
        if (ImGui::ColorEdit3("Foam Color", foam)) {
            params.foamColor = { (unsigned char)(foam[0] * 255), (unsigned char)(foam[1] * 255), (unsigned char)(foam[2] * 255), 255 };
        }
        ImGui::SliderFloat("Foam Threshold", &params.foamThreshold, 0.0f, 1.5f);
        ImGui::SliderFloat("Foam Softness", &params.foamSoftness, 0.0f, 1.0f);
        ImGui::SliderFloat("Foam Pattern Scale", &params.foamPatternScale, 0.1f, 10.0f);
        ImGui::SliderFloat("Foam Blending", &params.foamBlending, 0.0f, 1.0f);
    }

    ImGui::End();
}