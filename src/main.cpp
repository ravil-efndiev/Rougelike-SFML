#include "Game.hpp"

i32 main(i32 argc, char* argv[]) {
    Game* game;
    if (argc > 1 && std::string(argv[1]) == "edit") {
        game = Game::create(Game::editor);
        game->run();
    }
    else {
        game = Game::create(Game::game);
        game->run();
    }
    delete game;
    return 0;
}
