#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"
#include "Water.hpp"
#include "UIManager.hpp"

class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    void Run();

private:
    void Update();
    void Draw();
    void LoadSkybox();

    Camera3D camera;
    Water waterObject;
    UIManager uiManager;

    Shader skyboxShader;
    Model skybox;
};

#endif // GAME_HPP