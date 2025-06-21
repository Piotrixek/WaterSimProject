#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include <imgui.h>
#include <rlgl.h>

const char* skyboxVS = R"(
#version 330
in vec3 vertexPosition;
out vec3 fragPosition;
uniform mat4 mvp;
uniform mat4 matModel;

void main()
{
    vec4 clipPos = mvp * matModel * vec4(vertexPosition, 1.0);
    gl_Position = clipPos.xyww;
    fragPosition = vertexPosition;
}
)";
const char* skyboxFS = R"(
#version 330
in vec3 fragPosition;
out vec4 finalColor;

void main()
{
    float y = normalize(fragPosition).y;
    vec3 topColor = vec3(0.0, 0.2, 0.5); // Darker blue
    vec3 bottomColor = vec3(0.5, 0.8, 1.0); // Lighter blue
    
    finalColor = vec4(mix(bottomColor, topColor, smoothstep(0.0, 0.4, y)), 1.0);
}
)";


Game::Game(int width, int height, const char* title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    SetTargetFPS(60);

    uiManager.Setup();
    waterObject.Load();
    LoadSkybox();

    camera = { 0 };
    camera.position = Vector3{ 25.0f, 15.0f, 25.0f };
    camera.target = Vector3{ 0.0f, 2.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

Game::~Game() {
    UnloadModel(skybox);
    UnloadShader(skyboxShader);
    uiManager.Shutdown();
    CloseWindow();
}

void Game::LoadSkybox() {
    skyboxShader = LoadShaderFromMemory(skyboxVS, skyboxFS);

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    skybox = LoadModelFromMesh(cube);
    skybox.materials[0].shader = skyboxShader;
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

void Game::Update() {
    UpdateCamera(&camera, CAMERA_ORBITAL);
    waterObject.Update(uiManager);
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(SKYBLUE);

    BeginMode3D(camera);
    {
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        DrawModel(skybox, Vector3Zero(), 1.0f, WHITE);
        rlEnableBackfaceCulling();
        rlEnableDepthMask();

        waterObject.Draw(camera);
        DrawGrid(20, 5.0f);
    }
    EndMode3D();

    uiManager.Begin();
    {
        uiManager.DrawControls();
    }
    uiManager.End();

    DrawFPS(10, 10);

    EndDrawing();
}