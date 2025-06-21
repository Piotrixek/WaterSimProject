#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "raylib.h"

struct WaterParams {

    Vector2 waveDir1 = { 1.0f, 0.2f };
    float steepness1 = 0.25f;
    float wavelength1 = 25.0f;
    float speed1 = 1.0f;

    Vector2 waveDir2 = { -0.3f, 0.8f };
    float steepness2 = 0.2f;
    float wavelength2 = 8.0f;
    float speed2 = 1.8f;

    Vector2 waveDir3 = { 0.5f, 0.5f };
    float steepness3 = 0.1f;
    float wavelength3 = 3.5f;
    float speed3 = 2.2f;

    Vector2 waveDir4 = { 0.8f, -0.4f };
    float steepness4 = 0.08f;
    float wavelength4 = 2.5f;
    float speed4 = 1.5f;

    // Appearance
    Color deepColor = { 5, 20, 40, 255 };
    Color shallowColor = { 0, 100, 130, 255 };
    float colorBias = 0.15f;
    float specularFactor = 128.0f;

    // Foam Parameters
    Color foamColor = { 220, 240, 255, 255 };
    float foamThreshold = 0.4f;
    float foamSoftness = 0.2f;
    float foamPatternScale = 1.0f;
    float foamBlending = 0.65f;
};


class UIManager {
public:
    UIManager();
    void Setup();
    void Shutdown();
    void Begin();
    void End();
    void DrawControls();
    const WaterParams& GetParams() const;
private:
    WaterParams params;
};

#endif // UIMANAGER_HPP