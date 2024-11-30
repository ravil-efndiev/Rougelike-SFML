#include "Game.hpp"

int main() {
    Game* game = Game::create(800, 600, "raf");
    game->run();
    delete game;
    return 0;
}
