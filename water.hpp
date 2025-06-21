#ifndef WATER_HPP
#define WATER_HPP

#include "raylib.h"
#include "UIManager.hpp"

class Water {
public:
    Water();
    ~Water();

    void Load();
    void Update(const UIManager& ui);
    void Draw(Camera3D& camera);

    Shader GetShader();

private:
    Model waterModel;
    Shader waterShader;
    float time;

    int timeLoc;
    int waveDir1Loc, steepness1Loc, wavelength1Loc, speed1Loc;
    int waveDir2Loc, steepness2Loc, wavelength2Loc, speed2Loc;
    int waveDir3Loc, steepness3Loc, wavelength3Loc, speed3Loc;
    int waveDir4Loc, steepness4Loc, wavelength4Loc, speed4Loc;
    int deepColorLoc, shallowColorLoc, colorBiasLoc, specularFactorLoc;
    int viewPosLoc;
    int foamColorLoc, foamThresholdLoc, foamSoftnessLoc, foamPatternScaleLoc, foamBlendingLoc;
    int skyboxLoc;
};

#endif // WATER_HPP