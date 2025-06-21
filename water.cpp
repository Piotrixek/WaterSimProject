#include "Water.hpp"
#include "raymath.h"
#include <rlgl.h>

const char* waterVertexShader = R"(
#version 330
in vec3 vertexPosition; in vec3 vertexNormal;
uniform float time;
uniform vec2 waveDir1; uniform float steepness1; uniform float wavelength1; uniform float speed1;
uniform vec2 waveDir2; uniform float steepness2; uniform float wavelength2; uniform float speed2;
uniform vec2 waveDir3; uniform float steepness3; uniform float wavelength3; uniform float speed3;
uniform vec2 waveDir4; uniform float steepness4; uniform float wavelength4; uniform float speed4;
uniform mat4 mvp; uniform mat4 matModel;
out vec3 fragNormal; out vec3 fragPosition; out float fragHeight;
struct WaveResult { vec3 position; vec3 normal; };
WaveResult gerstnerWave(vec3 pos, vec2 dir, float steepness, float wavelength, float speed) {
    WaveResult r; dir = normalize(dir);
    float k = 2.0 * 3.1415926535 / wavelength;
    float c = sqrt(9.8 / k) * speed;
    float f = k * (dot(dir, pos.xz) - c * time);
    float a = steepness / k;
    r.position.x = a * dir.x * cos(f);
    r.position.y = a * sin(f);
    r.position.z = a * dir.y * cos(f);
    float wa = k * a;
    r.normal.x = -dir.x * wa * cos(f);
    r.normal.y = 1.0 - wa * sin(f);
    r.normal.z = -dir.y * wa * cos(f);
    return r;
}
void main()
{
    WaveResult r1 = gerstnerWave(vertexPosition, waveDir1, steepness1, wavelength1, speed1);
    WaveResult r2 = gerstnerWave(vertexPosition, waveDir2, steepness2, wavelength2, speed2);
    WaveResult r3 = gerstnerWave(vertexPosition, waveDir3, steepness3, wavelength3, speed3);
    WaveResult r4 = gerstnerWave(vertexPosition, waveDir4, steepness4, wavelength4, speed4);
    vec3 p = vertexPosition + r1.position + r2.position + r3.position + r4.position;
    vec3 n = normalize(vec3(0,1,0) - (r1.normal + r2.normal + r3.normal + r4.normal - vec3(0,4,0)));
    fragPosition = vec3(matModel * vec4(p, 1.0));
    gl_Position = mvp * vec4(p, 1.0);
    fragNormal = normalize(mat3(transpose(inverse(matModel))) * n);
    fragHeight = p.y;
}
)";


const char* waterFragmentShader = R"(
#version 330
in vec3 fragNormal;
in vec3 fragPosition;
in float fragHeight;

uniform vec3 deepColor;
uniform vec3 shallowColor;
uniform float colorBias;
uniform float specularFactor;
uniform vec3 viewPos;
uniform vec3 foamColor;
uniform float foamThreshold;
uniform float foamSoftness;
uniform float foamPatternScale;
uniform float foamBlending;
uniform samplerCube skybox;

out vec4 finalColor;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.y * u.x;
}

void main()
{
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 lightDir = normalize(vec3(0.8, 1.0, 0.7));

    vec3 reflectDir = reflect(-viewDir, norm);
    vec3 reflectionColor = texture(skybox, reflectDir).rgb;

    float fresnel = 0.02 + 0.98 * pow(1.0 - max(dot(viewDir, norm), 0.0), 5.0);
    
    vec3 waterColor = mix(deepColor, shallowColor, fragHeight + colorBias);
    vec3 baseColor = mix(waterColor, reflectionColor, fresnel);
    
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * baseColor;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfDir), 0.0), specularFactor);
    vec3 specular = spec * vec3(1.0, 1.0, 1.0);

    float heightFoam = smoothstep(foamThreshold - foamSoftness, foamThreshold + foamSoftness, fragHeight);
    float noiseFoam = noise(fragPosition.xz * foamPatternScale);
    float foamAmount = heightFoam * noiseFoam;

    vec3 litWaterColor = baseColor * (ambient + diffuse) + specular;



    vec3 blendedFoamColor = mix(foamColor, litWaterColor, foamBlending);


    vec3 final = mix(litWaterColor, blendedFoamColor, foamAmount);
    
    finalColor = vec4(final, 1.0);
}
)";


Water::Water() : time(0.0f) {}

Water::~Water() {
    UnloadModel(waterModel);
    UnloadShader(waterShader);
}

void Water::Load() {
    waterShader = LoadShaderFromMemory(waterVertexShader, waterFragmentShader);

    timeLoc = GetShaderLocation(waterShader, "time");
    waveDir1Loc = GetShaderLocation(waterShader, "waveDir1");
    steepness1Loc = GetShaderLocation(waterShader, "steepness1");
    wavelength1Loc = GetShaderLocation(waterShader, "wavelength1");
    speed1Loc = GetShaderLocation(waterShader, "speed1");
    waveDir2Loc = GetShaderLocation(waterShader, "waveDir2");
    steepness2Loc = GetShaderLocation(waterShader, "steepness2");
    wavelength2Loc = GetShaderLocation(waterShader, "wavelength2");
    speed2Loc = GetShaderLocation(waterShader, "speed2");
    waveDir3Loc = GetShaderLocation(waterShader, "waveDir3");
    steepness3Loc = GetShaderLocation(waterShader, "steepness3");
    wavelength3Loc = GetShaderLocation(waterShader, "wavelength3");
    speed3Loc = GetShaderLocation(waterShader, "speed3");
    waveDir4Loc = GetShaderLocation(waterShader, "waveDir4");
    steepness4Loc = GetShaderLocation(waterShader, "steepness4");
    wavelength4Loc = GetShaderLocation(waterShader, "wavelength4");
    speed4Loc = GetShaderLocation(waterShader, "speed4");
    deepColorLoc = GetShaderLocation(waterShader, "deepColor");
    shallowColorLoc = GetShaderLocation(waterShader, "shallowColor");
    colorBiasLoc = GetShaderLocation(waterShader, "colorBias");
    specularFactorLoc = GetShaderLocation(waterShader, "specularFactor");
    viewPosLoc = GetShaderLocation(waterShader, "viewPos");
    foamColorLoc = GetShaderLocation(waterShader, "foamColor");
    foamThresholdLoc = GetShaderLocation(waterShader, "foamThreshold");
    foamSoftnessLoc = GetShaderLocation(waterShader, "foamSoftness");
    foamPatternScaleLoc = GetShaderLocation(waterShader, "foamPatternScale");
    foamBlendingLoc = GetShaderLocation(waterShader, "foamBlending");
    skyboxLoc = GetShaderLocation(waterShader, "skybox");

    Mesh waterMesh = GenMeshPlane(150, 150, 200, 200);
    waterModel = LoadModelFromMesh(waterMesh);
    waterModel.materials[0].shader = waterShader;
}

void Water::Update(const UIManager& ui) {
    time += GetFrameTime();
    const WaterParams& params = ui.GetParams();


    SetShaderValue(waterShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, waveDir1Loc, &params.waveDir1, SHADER_UNIFORM_VEC2);
    SetShaderValue(waterShader, steepness1Loc, &params.steepness1, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, wavelength1Loc, &params.wavelength1, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, speed1Loc, &params.speed1, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, waveDir2Loc, &params.waveDir2, SHADER_UNIFORM_VEC2);
    SetShaderValue(waterShader, steepness2Loc, &params.steepness2, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, wavelength2Loc, &params.wavelength2, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, speed2Loc, &params.speed2, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, waveDir3Loc, &params.waveDir3, SHADER_UNIFORM_VEC2);
    SetShaderValue(waterShader, steepness3Loc, &params.steepness3, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, wavelength3Loc, &params.wavelength3, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, speed3Loc, &params.speed3, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, waveDir4Loc, &params.waveDir4, SHADER_UNIFORM_VEC2);
    SetShaderValue(waterShader, steepness4Loc, &params.steepness4, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, wavelength4Loc, &params.wavelength4, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, speed4Loc, &params.speed4, SHADER_UNIFORM_FLOAT);

    Vector3 deepNorm = { params.deepColor.r / 255.f, params.deepColor.g / 255.f, params.deepColor.b / 255.f };
    Vector3 shallowNorm = { params.shallowColor.r / 255.f, params.shallowColor.g / 255.f, params.shallowColor.b / 255.f };
    Vector3 foamNorm = { params.foamColor.r / 255.f, params.foamColor.g / 255.f, params.foamColor.b / 255.f };
    SetShaderValue(waterShader, deepColorLoc, &deepNorm, SHADER_UNIFORM_VEC3);
    SetShaderValue(waterShader, shallowColorLoc, &shallowNorm, SHADER_UNIFORM_VEC3);
    SetShaderValue(waterShader, foamColorLoc, &foamNorm, SHADER_UNIFORM_VEC3);
    SetShaderValue(waterShader, foamPatternScaleLoc, &params.foamPatternScale, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, foamBlendingLoc, &params.foamBlending, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, colorBiasLoc, &params.colorBias, SHADER_UNIFORM_FLOAT);
    SetShaderValue(waterShader, specularFactorLoc, &params.specularFactor, SHADER_UNIFORM_FLOAT);
}

void Water::Draw(Camera3D& camera) {
    SetShaderValue(waterShader, viewPosLoc, &camera.position, SHADER_UNIFORM_VEC3);
    DrawModel(waterModel, Vector3Zero(), 1.0f, WHITE);
}


Shader Water::GetShader() {
    return waterShader;
}