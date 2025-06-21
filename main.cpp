#include "game.hpp"

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    Game game(screenWidth, screenHeight, "Realistic Water Simulation");
    game.Run();

    return 0;
}